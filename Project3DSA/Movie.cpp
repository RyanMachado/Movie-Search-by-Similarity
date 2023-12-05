//
// Created by Aweso on 12/2/2023.
//

#include "Movie.h"

//constructs a movie object given parameters from csv readin
Movie::Movie(int m, string t, int y, string g, float r) {
    movieid = m;
    title = t;
    year = y;
    while(g.size() > 0) {
        genre.push_back(g.substr(0, g.find('|')));
        if(g.find('|') != string::npos) {
            g.erase(0, (g.find('|') + 1));
        } else {
            g = "";
        }
    }
    rating = r;
    similarity = 0;
}

//constructs a movie when genre is given as a vector of string
Movie::Movie(int m, string t, int y, vector<string> &v, float r, int s) {
    movieid = m;
    title = t;
    year = y;
    genre = v;
    rating = r;
    similarity = s;
}

void Movie::setRating(float rating) {
    this->rating = rating;
}

string Movie::getTitle() {
    return title;
}

int Movie::getSimilarity() {
    return similarity;
}

//sets the similarity class variable when given a comparison movie
void Movie::calcSimilarity(Movie &m) {
    int count = 0;
    similarity = 0;
    if(genre.size() == 1){
        if(find(m.genre.begin(), m.genre.end(), genre[0]) != m.genre.end()){
            similarity += 40;
        }
    } else if(genre.size() == 2){
        if(find(m.genre.begin(), m.genre.end(), genre[0]) != m.genre.end()){
            similarity += 30;
        }
        if(find(m.genre.begin(), m.genre.end(), genre[1]) != m.genre.end()){
            similarity += 10;
        }
    } else {
        for(int i=0; i<genre.size(); i++){
            if(find(m.genre.begin(), m.genre.end(), genre[i]) != m.genre.end()){
                if(count == 0){
                    similarity += 24;
                    count++;
                } else if(count == 1){
                    similarity += 11;
                    count++;
                } else {
                    similarity += 5;
                    count++;
                }
            }
        }
    }
    if(abs(m.year - year) <= 3){
        similarity += 10;
    } else if(abs(m.year - year) <= 10){
        similarity += 5;
    }
    similarity += (int)round(rating * 10);
    if(m.getTitle() == title){
        similarity = 0;
    }
}

float Movie::getRating() {
    return rating;
}

//sets a Movie object equal to another movie object
Movie Movie::operator=(Movie const &movie){
    this->movieid = movie.movieid;
    this->title = movie.title;
    this->year = movie.year;
    this->genre = movie.genre;
    this->rating = movie.rating;
    this->similarity = movie.similarity;
    return *this;
}

void Movie::swapMovies(Movie &movie){
    Movie temp = *this;
    *this = movie;
    movie = temp;
}