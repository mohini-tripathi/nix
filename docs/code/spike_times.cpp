// clang++ -std=c++11 -O2 spike_times.cpp -o spike_times
#include <nix.hpp>
#include <chrono>
#include <iostream>
#include <random>


double store_spike_times(nix::DataArray &array, double dt, double spike_prob, size_t chunksize=1000) {
    auto start = std::chrono::high_resolution_clock::now();


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    return elapsed.count();
}

double store_binary_spikes(nix::DataArray &array, double dt, double spike_prob, size_t chunksize=1000) {
    auto start = std::chrono::high_resolution_clock::now();


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    return elapsed.count();
}


void create_spike_times() {
    /*
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (int n = 0; n < 10; ++n) {
        // Use dis to transform the random unsigned int generated by gen into a
        // double in [1, 2). Each call to dis(gen) generates a new random double
        std::cout << dis(gen) << ' ';
    }
    std::cout << '\n';
    */
}

void test_times(size_t runs, double duration, double dt, size_t trials=10, size_t chunksize=1000) {
    for (size_t i = 0;i < runs; ++i) {
        //std::string filename = "Spike_time_benchmark_times_" + nix::util::numToStr(i) + ".nix";
        nix::File f_times = nix::File::open("test.nix", nix::FileMode::Overwrite);//, "hdf5",
        //                                            nix::Compression::DeflateNormal);
        nix::Block b_times = f_times.createBlock("Test", "test");
        //nix::NDSize initial_shape(1, chunksize);
        // nix::DataArray da_times = b_times.createDataArray("spike times", "nix.event.spike_times",
        //nix::DataType::Double, initial_shape);

        f_times.close();
    }
}

int main(void) {
    test_times(1, 10.0, 0.0005, 10, 1000);

    return 0;
}