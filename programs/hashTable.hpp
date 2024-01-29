#pragma once
#include <iostream>
#include <unordered_set>
#include <unordered_map>

#include "pthash/include/pthash.hpp"

using namespace pthash;

/* Declare the PTHash function. */
typedef single_phf<murmurhash2_64,         // base hasher
                   dictionary_dictionary,  // encoder type
                   true                    // minimal
                   >
    pthash_type;

struct createPthashOutput {
    pthash_type f;
    unordered_map<u_int64_t, int, pthash_type> hashTable;
};

createPthashOutput createPthash(vector<pair<long long, int>>& keyValues) {

    unordered_set<long long> keys;
    for (auto& keyValue : keyValues) {
        keys.insert(keyValue.first);
    }

    for (int i = 0; i < 20; ++i) {
        keys.insert(i); // if there are too little keys the pthash breaks
    }
    vector<uint64_t> pthashKeys;
    for (auto& key : keys) {
        pthashKeys.push_back(key);
        // cout<< key << " ";
    }
    // cout<<endl;
    keys.clear();
    
    pthash_type f;

    build_configuration config;
    config.c = 11.0;
    config.alpha = 0.84;
    config.minimal_output = true;  // mphf
    config.verbose_output = false;

    
    // cout << "build pthash" << endl;

    f.build_in_internal_memory(pthashKeys.begin(), pthashKeys.size(), config);

    // cout << "create hashmap" << endl;

    unordered_map<u_int64_t, int, pthash_type> hashTable{pthashKeys.size(),f};

    for (auto& keyValue : keyValues) {
        hashTable[keyValue.first] = keyValue.second;
    }

    // for (auto& key : keys) {
    //     cout << "f(" << key << ") = " << f(key) << endl;
    // }

    createPthashOutput result;
    result.f = f;
    result.hashTable = hashTable;

    return result;

}