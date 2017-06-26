#include <iostream>
#include <fstream>
#include <thread>

#include <unistd.h> // for getpass

#include "constants.h"
#include "header.h"
#include "encrypt_decrypt.h"

using namespace std;

string merge_files(const string &directory, const string &merged_file){
    string command = "cd " + directory + " && " + "cat * > " + merged_file;
    return command;
}

int main(int argc, char* argv[]){
    if (argc < 2){
        cerr << "not enough arguments" << endl;
        exit(1);
    }

    //todo: perform sanity check on file name (argv[argc - 1])

    bool perform_encryption = false;
    bool perform_decryption = false;
    string password;

    bool compress = false;
    bool decompress = false;

    string input_file;
    string output_file;

    for (int i = 1; i < argc - 1; i++){
        string s = argv[i];

        if (s == "-e" || s == "--encrypt")
            perform_encryption = true;
        else if (s == "-d" || s == "--decrypt")
            perform_decryption = true;
        else if (s == "-c" || s == "--compress")
            compress = true;
        else if (s == "-dc" || s == "--decompress")
            decompress = true;
        else if (s == "-in"){
            input_file = argv[i + 1];
            i++;
        }
        else if (s == "-out"){
            output_file = argv[i + 1];
            i++;
        }
        else{
            cerr << "argument error: unexpected argument" << endl;
            exit(1);
        }
    }

    if ((compress xor decompress) == 0){
        cerr << "argument error: both -c and -dc are present or are missing" << endl;
        exit(1);
    }

    if (perform_encryption && perform_decryption){
        cerr << "argument error: you cannot both encrypt and decrypt the file" << endl;
        exit(1);
    }

    if (input_file.empty()){
        cerr << "argument error: no input file" << endl;
        exit(1);
    }

    // output_file must not be empty
    if (output_file.empty()){
        output_file = input_file;
    }

    // read password for encryption/decryption
    if (perform_encryption || perform_decryption){
        password = getpass("Password: ");
    }

    // open input file
    const string file_name = input_file;
    ifstream input(file_name, std::ios_base::binary);

    // Stop eating new lines in binary mode!!!
    input.unsetf(std::ios::skipws);

    // create directory used for temporary files
    const auto directory = file_name + "_scofield";
    system(("rm -rf " + directory).c_str());
    system(("mkdir " + directory).c_str());

    // get length of the file
    input.seekg(0, input.end);
    const auto FILE_LENGTH = (size_t) input.tellg();
    input.seekg(0, input.beg);

    size_t MAX_READ;
    char *buffer = nullptr;

    if (compress){
        // set MAX_READ - the maximum number of bytes we can read at one point
        if (FILE_LENGTH <= MAX_LENGTH) {
            MAX_READ = FILE_LENGTH;
        } else {
            const auto MAX_NUMBER_THREADS = thread::hardware_concurrency();
            MAX_READ = (FILE_LENGTH / MAX_NUMBER_THREADS);
        }

        // allocate new buffer (used for reading all input)
        buffer = new char[MAX_READ + 1];

        // index of file
        size_t nr = 0;

        vector<thread> threads;
        vector<size_t> file_sizes; // used for header

        while (!input.eof()) {
            input.read(buffer, MAX_READ);
            string buff(buffer, (size_t)input.gcount());

            file_sizes.push_back((size_t)input.gcount());

            thread t([=] {
                const auto file = directory + "/" + to_string(nr);

                ofstream out(file);
                string compressed = buff;

                if (perform_encryption){
                    encrypt(password, compressed);
                }

                out << compressed;
                out.close();
            });

            nr++;
            threads.push_back(move(t));
        }

        // at most (thread::hardware_concurrency() + 1) threads
        for (auto &t: threads)
            t.join();

        // merge all files into one
        system(merge_files(directory, "merged_files.txt").c_str());

        // create header
        const auto header1 = directory + "/" + "header.txt";
        system(("rm -f " + header1).c_str());
        ofstream hdr(header1);
        hdr << make_standard_header(file_sizes).to_string();
        hdr << endl;
        hdr.close();

        // create final file and delete directory
        system(("cat " + directory + "/header.txt " + directory + "/merged_files.txt > " + output_file +
                ".scofield").c_str());
        system(("rm -rf " + directory).c_str());
    }
    else{
        // read header if necessary
        Header header = read_standard_header(input);

        MAX_READ = header.file_sizes[0];

        // allocate new buffer (used for reading all input)
        buffer = new char[MAX_READ + 1];

        // index of file
        size_t nr = 0;

        vector<thread> threads;

        while (nr < header.number_files) {
            input.read(buffer, header.file_sizes[nr]);
            string buff(buffer, (size_t)input.gcount());

            thread t([=] {
                const auto file = directory + "/" + to_string(nr);

                ofstream out(file);
                string decompressed = buff;

                if (perform_decryption){
                    decrypt(password, decompressed);
                }

                out << decompressed;
                out.close();
            });

            nr++;
            threads.push_back(move(t));
        }

        // at most (thread::hardware_concurrency() + 1) threads
        for (auto &t: threads)
            t.join();

        // merge all files into one
        system(merge_files(directory, "merged_files.txt").c_str());

        system(("cp " + directory + "/merged_files.txt" + " " + output_file).c_str());
        system(("rm -rf " + directory).c_str());
    }

    input.close();
    delete[] buffer;

    return 0;
}
