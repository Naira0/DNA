#include <sstream>
#include <stdexcept>
#include "string.hpp"

String String::substr(size_t pos, size_t count) const
{
	size_t i, j;
    String store(count+1);

	if (pos >= m_size || pos + count > m_size)
		return String();

	for (i = pos, j = 0; j < count; i++, j++)
	{
		store += m_data[i];
	}

	store[j] = '\0';

	return std::move(store);
}

String String::substr(size_t pos) const
{ 
	if (pos >= m_size)
		return (m_data + (m_size-1));
	return m_data + pos;
}

std::string_view String::substr_view(size_t pos) const
{
    if (pos >= m_size)
        return (m_data + (m_size-1));
    return m_data + pos;
}

void String::reserve(size_t new_size)
{
	char* temp = new char[new_size];

	strcpy(temp, m_data);

	delete[] m_data;

	m_data = temp;
	m_capacity = new_size;
}

void String::shrink_to_fit()
{
	m_capacity = m_size + 1;

	char* temp = new char[m_capacity];

	strcpy(temp, m_data);

	delete[] m_data;
	m_data = temp;
}

void String::clear() { reassign(""); }

String& String::operator=(const char* str) { reassign(str); return *this; }

void String::operator+=(const char* str)
{
	m_size += strlen(str);
	if (m_size >= m_capacity)
		grow();
	strcat(m_data, str);
}

void String::operator+=(String& str)
{
	m_size += str.m_size;
	if (m_size >= m_capacity)
		grow();
	strcat(m_data, str.m_data);
}

void String::operator+=(const char c)
{
	m_size += 1;

	if (m_size >= m_capacity)
		grow();

	char toStr[] = { c, '\0' };

	strcat(m_data, toStr);
}

String String::operator+(const char* str) const
{
	size_t len = (m_size + strlen(str)) + 2;
	char* temp = new char[len];

	strcpy(temp, m_data);
	strcat(temp, str);

	return String(temp, true);
}

String String::operator+(String& str) const
{
	return (*this + str.m_data);
}

bool String::starts_with(const char* str) const
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (m_data[i] != str[i])
			return false;
	}
	return true;
}

bool String::ends_with(const char* str) const
{
	size_t len = m_size - strlen(str);
	for (size_t i = len; str[i] != '\0'; i++)
	{
		if (m_data[i] != str[i])
			return false;
	}
	return true;
}

bool String::contains(const char c) const
{
	for (size_t i = 0; i < m_size; i++)
	{
		if (m_data[i] == c)
			return true;
	}

	return false;
}

bool String::contains(const char* str) const
{
	bool start = true;
	int  matches = 0;

	// j will only increment when it finds the start of the substr otherwise it will be set to zero
	size_t  i, j;

	for (i = 0, j = 0; i <= m_size; i++, start ? j++ : 0)
	{
		// second part prevents matching against repeating characters
		if (m_data[i] == str[0] && m_data[i + 1] == str[j + 1])
			start = true;

		if (start && m_data[i] == str[j])
			matches++;
		else
			start = false;

		if (str[matches + 1] == '\0')
			return true;
	}

	return false;
}

inline char& String::get_element(size_t index) const
{
	if (index > m_size)
		throw std::out_of_range("Index out of range");
	return m_data[index];
}

template<typename T>
inline bool  String::cmp(T& str) const
{
	for (size_t i = 0; i < m_size; i++)
	{
		if (m_data[i] != str[i])
			return false;
	}
	return true;
}

inline void String::reassign(const char* str)
{
	m_size = strlen(str);

	if (m_size >= m_capacity)
		m_capacity *= 2;

	delete[] m_data;

	m_data = new char[m_capacity];
	strcpy(m_data, str);
}

inline void String::grow()
{
	m_capacity *= 2;

	char* temp = new char[m_capacity];

	strcpy(temp, m_data);

	delete[] m_data;
	m_data = temp;
}

bool String::operator==(String& str) const { return cmp(str); } 
bool String::operator==(const char* str) const { return cmp(str); }

char& String::operator[](size_t index) { return get_element(index); }
char  String::operator[](size_t index) const { return get_element(index); }

char& String::at(size_t index) { return get_element(index); }
char  String::at(size_t index) const { return get_element(index); }

void String::repeat(char c, size_t amount)
{
	for (int i = 0; i <= amount; i++)
	{
		*this += c;
	}
}

void String::lower()
{
	for (int i = 0; i < m_size; i++)
	{
		m_data[i] = tolower(m_data[i]);
	}
}

void String::upper()
{
	for (int i = 0; i < m_size; i++)
	{
		m_data[i] = toupper(m_data[i]);
	}
}

void String::trim_lead()
{
	if (m_data[0] != ' ')
		return;

	int i = 0;
	while (i < m_size && m_data[i++] == ' ') {};
	i -= 1;

	strcpy(m_data, m_data + i);

	m_size -= i;
}

void String::trim_trail()
{
	if (m_data[m_size - 1] != ' ')
		return;

	int i =  m_size-1;
	while (i < m_size && m_data[i--] == ' ') {};
	i += 2;

	strncpy(m_data, m_data, i);
	m_size = i;
}

void String::trim()
{
	if (m_data[0] != ' ' || m_data[m_size - 1] != ' ')
		return;

	size_t i = 0, j = m_size-1;

	while (i < m_size&& m_data[i++] == ' ') {};
	i -= 1;

	while (j < m_size && m_data[j--] == ' ') {};
	j += 2;

	strncpy(m_data, m_data + i, j-i);
	m_size = j-i;
}

size_t String::index_of(const char c, size_t offset)
{
	if (offset >= m_size)
		offset = 0;

	for (size_t i = offset; i < m_size; i++)
	{
		if (m_data[i] == c)
			return i;
	}

	return 0;
}

std::vector<String> String::split(char delim) const
{
	std::vector<String> result;
    size_t start = 0, current = 0;

	for (const char c : *this)
	{
        current++;

        if (c == delim ||  current == m_size)
		{
            size_t offset = current == m_size ? current-start : current-start-1;
            String str = substr(start, offset);

			result.emplace_back(std::move(str));

            start = current;
		}
	}
	return result;
}

void String::erase(size_t index)
{
    if(index >= m_size)
        return;

    for(size_t i = index; i < m_size; i++)
    {
        m_data[i] = m_data[i+1];
    }

    m_size -= 1;
}

void String::insert(size_t index, const char c)
{
    if(index >= m_size)
        return;

    if(m_size+1 >= m_capacity)
        grow();

    char last = m_data[m_size-1];

    for(size_t i = m_size-1; i >= index; i--)
    {
        if(i == index)
            m_data[index] = c;
        else
            m_data[i] = m_data[i-1];
    }

    m_data[m_size] = last;
    m_data[m_size+1] = '\0';

    m_size += 1;
}

String String::slice(size_t start, size_t end) const
{
    if(start >= m_size || end >= m_size)
        return String();

    char *temp = new char[m_size+1];

    strncpy(temp, m_data+start, (end-start)+1);

    return temp;
}



