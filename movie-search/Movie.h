//
// Created by Aweso on 12/2/2023.
//

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
#pragma once

class Movie {
private:
    int movieid;
    string title;
    int year;
    vector<string> genre;
    float rating;
    int similarity;
public:
    Movie(int m, string t, int y, string g, float r);
    Movie(int m, string t, int y, vector<string> &v, float r, int s);

    float getRating();
    string getTitle();
    int getSimilarity();

    void setRating(float rating);

    void calcSimilarity(Movie &m);
    Movie operator=(Movie const &movie);
    void swapMovies(Movie &movie);

};

