#pragma once


#include <iostream>

#include <set>

#include <map>

#include <sstream>

#include <fstream>

#include <string>

#include <cctype>

#include <vector>

#include <algorithm>

#include <iterator>

#include <stdio.h>

#include <string.h>

using namespace std;



//Helper function: checks if the string contains at least 1 alphabetic character.

//If it doesn't contain at least 1 alphabetic character => returns an empty string.

string alphaChecker(string& s, string& cleanedToken, bool& length){


    int count = 0;

    for(int i = 0; i < s.size(); i++){

        if(isalpha(s[i])){

            count++;

        }

        else{

            count = count;

        }

    }


    for(char c: cleanedToken){

        if(isalpha(c)){

        length = true;

        c = tolower(c);

        }

    }


    if(!length){

        return "";

    }

    else{

        return cleanedToken;

    }


    if(count == 0){

        //s = "";

        s;

    }

}



//Helper function: check the cleanedToken string in reverse to determine punctuation

void reverseChecker(string& s, string& cleanedToken){


    int pos = cleanedToken.size() - 1;


    while(pos >= 0){

        if(ispunct(cleanedToken[pos])){

            cleanedToken.pop_back();

        }

        else{

            break;

        }

        --pos;

    }


}



//Helper function: removes the question/exclamation marks + extra leading/trailing dots

void removeExclamation(string& s){


     //Removes the exclamation marks

    size_t pos = s.find_first_not_of("!");

    s.erase(0, pos);

  

    pos = s.find_last_not_of("!");


    if(string::npos != pos){

        s.erase(pos + 1);

    }


    //Removes the question marks

    size_t pos2 = s.find_first_not_of("?");

    s.erase(0, pos2);

  

    pos2 = s.find_last_not_of("?");


    if(string::npos != pos2){

        s.erase(pos2 + 1);

    }


    //Removes the extra dots just in case the eraser() will fail to complete the job properly

    size_t pos3 = s.find_first_not_of(".");

    s.erase(0, pos3);

  

    pos3 = s.find_last_not_of(".");


    if(string::npos != pos3){

        s.erase(pos3 + 1);

    }

}



//Helper function => removes the leading/trailing punctuation

string eraser(string& s){

   

    bool length = false;

    int size = s.size();

    string cleanedToken;

    int occurences = 0;

    

    for(int i = 0; i < size; ++i){

        if(ispunct(s[i])){

           occurences++;

        }

        else{

            break;

        }

    } 

    

    //Returns a newly constructed string with its value initialized to a copy of a substring of this object

    cleanedToken = s.substr(occurences, size - occurences + 1);

    //Allows to check the string in reverse to determine punctuation

    reverseChecker(s, cleanedToken);


    //Sets all elements from cleantedToken to lower if isalpha() true => then returns the result

    return alphaChecker(s, cleanedToken, length);

}



//Helper function => removes the leading/trailing spaces

void removeSpaces(string& s){

    

    size_t pos = s.find_first_not_of(" \t");

    s.erase(0, pos);

  

    pos = s.find_last_not_of(" \t");


    if(string::npos != pos){

        s.erase(pos + 1);

    }

}  



//Step 1: using the above helper functions, cleanToken() accepts 1 string as input

//And removes all leading and trailing punctuation, and then returns the cleaned version of the string.

string cleanToken(string s){


    string cleanedToken;

    cleanedToken = eraser(s);

    removeSpaces(cleanedToken);

    removeExclamation(cleanedToken);


    return cleanedToken;

}



//Step 2: implement the gatherTokens() function to gather the tokens from the input text after "tokenizing" the input text

set <string> gatherTokens(string text){


    set <string> tokens;

    vector <string> content;

    string token;

    stringstream tokenizer(text); 


    while(getline(tokenizer, token, ' ')){ 

       content.push_back(cleanToken(token));

    }


    for(string c: content){

        if(cleanToken(c) != ""){

            tokens.insert((c));

        }

    }


    return tokens;

}



//Step 3: implement the buildIndex() function => constructs an inverted index from the contents of the database file.

int buildIndex(string filename, map<string, set<string>>& index){


    ifstream userFile;

    string fileContents;

    int count = 0;


    //Initialize the container where you will gather your tokens

    set <string> tokenHolder;

    vector <string> contentHolder;


    //Open the file

    userFile.open(filename.c_str());


    //Odd number lines = URLs

    //Must start with one => TA

    int step = 1;

  

    //Check if the file is open or not

    if(!userFile.is_open()){

        cout << "Error: unable to open '" << filename << "'" << endl;

        return 0;


    }

    else{

        if(!userFile.fail() && !userFile.eof()){

            do{

                if(fileContents.size() > 0){

                    contentHolder.push_back(fileContents);

                }

            }while(getline(userFile, fileContents, '\n'));

        }

    }


    int length = contentHolder.size();


    for(int i = 0; i < length; i++){

        if(i % 2 == 0){

           count++;

            //Gather tokens from the index i + 1 stored inside the vector => where the lines are stored => even index

            tokenHolder = gatherTokens(contentHolder[i + 1]); 

            for(auto c: tokenHolder){

                //Insert the URLs at the given index => the index for URLs is odd

                index[c].insert(contentHolder[i]); 

           }

        }

        else{

           count = count;

        }

    }


    //Close the file

    userFile.close();

    

    return count;

}



//Helper function => finds the union using the algorithm described in the C++ library elsewhere

set <string> unionSet(map<string, set<string>>& index, set<string>& outputSet,  set<string>& junkHolder, set <string>& URL, stringstream& tokenizer,  string& tokenContents, string& token){


        for(auto c: index[token]){

            if(index.count(token) == 1){

                outputSet.insert(c);

            }

            else{

                outputSet = outputSet;

            }

        }


    return outputSet;


}



//Helper function => finds the intersection using the algorithm described in the C++ library elsewhere

set <string> intersection(map<string, set<string>>& index, set<string>& outputSet,  set<string>& junkHolder,  stringstream& tokenizer,  string& tokenContents, string& sentence, string& token){


    set <string> URL;

    //Clean the set each time you call the function (TA)

    junkHolder.clear();


    for(auto c: index[token]){

        if (index.count(token) == 1){

            junkHolder.insert(c);

        } 

    }

    

    set_intersection(outputSet.begin(), outputSet.end(), junkHolder.begin(), junkHolder.end(), std::inserter(URL, URL.begin()));

    outputSet = URL;

    

    return outputSet;

}



//Helper function => finds the difference using the algorithm described in the C++ library elsewhere

set <string> difference(map<string, set<string>>& index, set<string>& outputSet,  set<string>& junkHolder, set <string>& URL, stringstream& tokenizer,  string& tokenContents, string& token){


    //Clean the set each time you call the function (TA)

    junkHolder.clear();

   

    for(auto c: index[token]){

        if(index.count(token) == 1){

            outputSet.erase(c);

        }

        else{

           outputSet = outputSet;

        }

    }


    return outputSet;

}



//Helper function => the main driver of the findQueryMatches() => launches difference(), intersection(), unionSet().

void engine(string& tokenContents, map<string, set<string>>& index, set<string>& outputSet, set<string>& junkHolder, set <string>& URL, stringstream& tokenizer, string& sentence){


        //DIFFERENCE => see the function description

        if (tokenContents[0] == '-'){

            string token = cleanToken(tokenContents);

            outputSet = difference(index, outputSet, junkHolder, URL, tokenizer, tokenContents, token);

        }


        //INTERSECTION => see the function description

        else if (tokenContents[0] == '+' && !(outputSet.size() == 0)){

            string token = cleanToken(tokenContents);

            outputSet = intersection(index, outputSet, junkHolder, tokenizer, tokenContents, sentence, token);   

        }


        //UNION => see the function description

        else{

            string token = cleanToken(tokenContents);

            outputSet = unionSet(index, outputSet, junkHolder, URL, tokenizer, tokenContents, token);

        }


}

 


//Step 4: findQueryMatches() => for each query entered by the user, it finds the matching pages and prints the URLs

set<string> findQueryMatches(map<string, set<string>>& index, string sentence){


    set <string> outputSet;

    vector <string> values;

    set <string> junkHolder;

    set <string> URL;


    stringstream tokenizer(sentence);

    string tokenContents;


    do{

        engine(tokenContents, index, outputSet, junkHolder, URL, tokenizer, sentence);


    }while(tokenizer >> tokenContents);

        

    return outputSet;

}



//Helper function for the creative component => allows to search for the key inside the first vector recursively

int recursiveSearch(vector <string>& finalToken, int& size, string& key){


    vector <int> index;

    size--;

    

    if(size < 0){

        cout << "The key was not found." << endl;

        return -1;

    }

  

    if(finalToken.at(size) == key){

        cout << "The key was found at: " << size << endl;

        return size;

    }

  

    return recursiveSearch(finalToken, size, key);

}



//Step 6: Phrase search() => my plan was to implement this function which searches for the phrase inside a document uploaded to a vector.

//The it was supposed to keep track of where in each document a phrase is, and then use that information to determine 

//if words in a phrase are next to each other in any particular document.

//BUT I couldn't figure out the solution (I am taking 7 classes. Too exhausted. I am so sorry. I tried really hard to complete this project.)

int creativeComponent(string filename, string key){


    ifstream userFile;

    string fileContents;

    int count = 0;


    //Initialize the container where you will gather your tokens

    vector <string> contentHolder;

    vector <string> finalToken;

    vector <int> index;


    //Open the file

    userFile.open(filename.c_str());

  

    //Check if the file is open or not

    if(!userFile.is_open()){

        cout << "Error: unable to open '" << filename << "'" << endl;

        exit(0);

    }

    else{

        if(!userFile.fail() && !userFile.eof()){

            while(getline(userFile, fileContents, '\n')){

                if(fileContents.size() > 0){

                    contentHolder.push_back(fileContents);

                }

            }

        }

    }



    for(int i = 0; i < contentHolder.size(); i++){

        const char *ptr = strstr(contentHolder.at(i).c_str(), key.c_str());

        if(ptr != nullptr){

            finalToken.push_back(contentHolder.at(i));

        }

    }


    int size = finalToken.size();

    int keyPosition;

    keyPosition = recursiveSearch(finalToken, size, key);


    //Close the file

    userFile.close();


    return keyPosition;

}



//Step 5: searchEngine() => constructs an inverted index from the contents of the database file.

//Displays how many web pages were processed to build the index and how many distinct words were found across all pages.

//Starts a loop that prompts the user to enter a query and for each query entered by the user, it finds the matching pages and prints the URLs.

//The program will be "alive" until a user enters '\n' or "".

void searchEngine(string filename){


    map<string, set <string>> index;

    string value, key;

    bool programAlive = false;

    set<string> finalSearch;

    int buildIndexCount = buildIndex(filename, index);


    cout << "Stand by while building index..." << endl;

    cout << "Indexed " << buildIndexCount << " pages containing " << index.size() << " unique terms" << endl;

    cout << endl;

    cout << "Enter query sentence (press enter to quit): ";


    getline(cin, value, '\n');


    do{

        programAlive = true;

        finalSearch = findQueryMatches(index, value);

        cout << "Found " << finalSearch.size() << " matching pages" << endl;


        for(auto c: finalSearch){

            cout << c << endl;

        }


        //cout << endl;

        //creativeComponent(filename, key);


        cout << endl;

        cout << "Enter query sentence (press enter to quit): ";


        getline(cin, value, '\n');


        if(value == "\n"){

            programAlive = false;

            exit(0);

        }


        if(value == ""){

            cout << "Thank you for searching!" << endl;

            programAlive = false;

            exit(0);

        }


    }while(value != "\n" && programAlive == true);

}


