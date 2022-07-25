class LFUCache
{
public:
    int cap;
    int minFreq;
    unordered_map<int, int> kv; // key -> value
    unordered_map<int, int> freq; // key -> freq
    unordered_map<int, list<int>> freqBuckets; // freq -> bucket of similar freq elements ordered by LFU in front.
    unordered_map<int, list<int>::iterator> itrList; // key -> iterator to element in bucket 
	                                                 //instead of searching it in the bucket, when moving it to next bucket)
	
    LFUCache(int capacity) : cap(capacity)
    {
    }

    void incrementFreq(int key)
    {
        auto &lfuList = freqBuckets[freq[key]];
        lfuList.erase(itrList[key]);
        if (minFreq == freq[key] && lfuList.size() == 0)
        {
            minFreq++;
        }

        freq[key] = freq[key] + 1;
        auto &newLfuList = freqBuckets[freq[key]];
        newLfuList.push_back(key);
        itrList[key] = prev(newLfuList.end());
    }

    int get(int key)
    {
        if (cap ==0 || minFreq == 0 || kv.find(key) == kv.end())
        {
            return -1;
        }
        incrementFreq(key);
        return kv[key];
    }

    void removeLfuElement()
    {
        if (cap == 0)
        {
            return;
        }
        auto &lfuList = freqBuckets[minFreq];
        int lfuValue = *lfuList.begin();
        itrList.erase(lfuValue);
        lfuList.erase(lfuList.begin());
        kv.erase(lfuValue);
        freq.erase(lfuValue);
    }

    void insertNewElement(int key, int value)
    {
        if (cap == 0)
            return;
        minFreq = 1;
        freq[key] = 1;
        kv[key] = value;

        auto &newLfuList = freqBuckets[freq[key]];
        newLfuList.push_back(key);
        itrList[key] = prev(newLfuList.end());
    }

    void put(int key, int value)
    {
        if (kv.find(key) != kv.end())
        {
			// existing value, use update key and freq.
            incrementFreq(key);
            kv[key] = value;
        }
        else
        {
            if (kv.size() == cap)
            {
			    // need to remove element
                removeLfuElement();
            }
			// insert fresh element
            insertNewElement(key, value);
        }
    }
};
