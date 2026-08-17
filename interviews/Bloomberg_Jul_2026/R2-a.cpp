/*
You have to implement two APIs, train and predict. The train API will take a vector of strings, or rather we can say a sentence. 
So the sentence will be broken in multiple strings of words, right. So what we have to do here is, we have to, like for the entire training
set, we have to predict the next word. So, for example, if we are getting two sentences where egg comes after green, and one sentence where
insect comes after green, then we have to return the word which has the highest frequency after that word. So both of the APIs can be 
called any time. We don't want to take care of multithreading here.
*/

struct FrequencyNode {
    int frequency;
    unordered_set<string> words;
};

struct WordData {

    // nextWord -> frequency
    unordered_map<string, int> frequency;

    // nextWord -> iterator to its frequency node
    unordered_map<string,
        list<FrequencyNode>::iterator> position;

    // Sorted by frequency, highest first.
    list<FrequencyNode> buckets;
};

class Predictor {

    // word -> information about its next words
    unordered_map<string, WordData> data;

public:

    void train(const vector<string>& sentence)
    {
        for (int i = 0; i + 1 < sentence.size(); ++i)
        {
            string word = sentence[i];
            string nextWord = sentence[i + 1];

            auto& info = data[word];

            int oldFreq = info.frequency[nextWord];
            int newFreq = oldFreq + 1;

            info.frequency[nextWord] = newFreq;

            // Find current frequency bucket.
            if (oldFreq > 0)
            {
                auto it = info.position[nextWord];

                it->words.erase(nextWord);

                if (it->words.empty())
                {
                    info.buckets.erase(it);
                }
            }

            // Find/create new frequency bucket.
            // Move nextWord to the appropriate bucket.
            //
            // Since frequency changes only from F -> F+1,
            // the bucket can be adjusted locally.

            auto newIt = /* iterator to frequency newFreq */;

            newIt->words.insert(nextWord);

            info.position[nextWord] = newIt;
        }
    }

    string predict(const string& word)
    {
        auto it = data.find(word);

        if (it == data.end())
            return "";

        auto& info = it->second;

        if (info.buckets.empty())
            return "";

        // Highest frequency bucket is at the front.
        return *info.buckets.front().words.begin();
    }
};
