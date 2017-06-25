#include <iostream>
#include <fstream>
#include <thread>
#include "burrows_wheeler_transform.h"
#include "constants.h"

using namespace std;

int main(int argc, char* argv[]){
    if (argc < 2){
        cerr << "not enough arguments" << endl;
        exit(1);
    }

    // open input file
    ifstream input(argv[1]);

    // create directory
    const auto directory = string(argv[1]) + "_scofield";
    system(("rm -rf " + directory).c_str());
    system(("mkdir " + directory).c_str());

    // get length of the file
    input.seekg (0, input.end);
    auto file_length = input.tellg();
    input.seekg (0, input.beg);

    if (file_length <= MAX_LENGTH){
        const auto MAX_READ = MAX_LENGTH;
        char buffer[MAX_READ + 1];

        input.read(buffer, MAX_READ);
        buffer[input.gcount() + 1] = '\0';

        ofstream out(directory + "/" + to_string(0));
        out << bwt(string(buffer));
        out.close();
    }
    else{
        const auto MAX_NUMBER_THREADS = thread::hardware_concurrency();
        const auto MAX_READ = file_length / MAX_NUMBER_THREADS;

        char buffer[MAX_READ + 1];
        int nr = 0;

        vector<thread> threads;

        while (!input.eof()){
            input.read(buffer, MAX_READ);
            buffer[input.gcount() + 1] = '\0';

            thread t([&, nr, directory]{
                ofstream out(directory + "/" + to_string(nr));
                out << bwt(string(buffer));
                out.close();
            });

            nr++;
            threads.push_back(move(t));
        }

        // at most (thread::hardware_concurrency() + 1) threads
        for (auto &t: threads)
            t.join();
    }

    return 0;
}
