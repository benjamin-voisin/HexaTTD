#pragma once

#include <vector>

class TrackUF {
    private:
        std::vector<int> parent;
        std::vector<int> rang;
        std::vector<int> classes;
        std::vector<int> classes_version;
        int n_current_classes;
        int n_classes;
        int version;


    public:
        TrackUF();
        int track_number();
        int add();
        void ens_union(int a, int b);
        
        // O(n) get the full a class
        std::vector<int> get_class(int a);
        // O(n) reset the full a class
        std::vector<int> del(int a);
        
        int find(int a);
        int classe(int a);
        int n_classe();
};