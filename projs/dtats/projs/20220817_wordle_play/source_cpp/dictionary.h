#ifndef DICTIONARY_H_INCLUDED
#define DICTIONARY_H_INCLUDED

class Dictionary
{
private:

public:
	Dictionary();
	~Dictionary();
	size_t Nelem();
	const char* GetStr(size_t in_which);
	unsigned GetPopularityRaw(size_t in_which);
	unsigned GetPopularityLinearized(size_t in_which);
};
#endif
