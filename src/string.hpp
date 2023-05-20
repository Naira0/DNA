#pragma once

#include <ostream>
#include <cstring>
#include <vector>
#include <memory>
#include <iostream>

class String
{
public:

	String(const char* data, bool move = false)
	{
		m_size = strlen(data);
		m_capacity = m_size <= 1 ? 10 : m_size * 2;

		if(move)
        {
		    m_data = const_cast<char*>(data);
		    data = nullptr;
        }
		else
        {
            m_data = new char[m_capacity];
            strcpy(m_data, data);
        }
        std::cout << "created\n";
	}

	String(const String& str)
	{
		m_size = str.m_size;
		m_capacity = str.m_capacity;

		m_data = new char[m_capacity];
		strcpy(m_data, str.m_data);
        std::cout << "created\n";
	}

    String(String&& str) noexcept
    {
        m_size = str.m_size;
        m_capacity = str.m_capacity;

        m_data = str.m_data;
        str.m_data = nullptr;

        std::cout << "moved!\n";
    }

	explicit String(size_t reserve_size = 10)
	{
		m_size = 0;
		m_capacity = reserve_size;
		m_data = new char[m_capacity];
		*m_data = '\0';
	}

	~String()
	{
		delete[] m_data;
	}

	const char*     data() const { return m_data; }
	char*			data()		 { return m_data; }

	const char* c_str() { return const_cast<const char*>(m_data); }

	size_t size() const { return m_size; }
	size_t capacity() const { return m_capacity; }

	char* begin() { return m_data; }
	char* end() { return &m_data[m_size]; }

	char* begin() const { return m_data; }
	char* end() const { return &m_data[m_size]; }

	bool empty() const { return m_size == 0; }

	void clear();

	void reserve(size_t new_size);

	void shrink_to_fit();

	String& operator=(const char* str);

	void operator+=(const char* str);
	
	void operator+=(String& str);

	void operator+=(const char c);

	String operator+(const char* str) const;
	String operator+(String& str) const;

	bool starts_with(const char* str) const;

	bool ends_with(const char* str) const;

	bool contains(const char c) const;

	bool contains(const char* str) const;

    String substr(size_t pos) const;
    String substr(size_t pos, size_t count) const;

    std::string_view substr_view(size_t pos) const;

	bool operator==(String& str) const;
	bool operator==(const char* str) const;

	char& operator[](size_t index);
	char  operator[](size_t index) const;

	char& at(size_t index);
	char  at(size_t index) const;

	friend std::ostream& operator<<(std::ostream& os, const String& str) 
	{
		return os << (!str.m_size ? "" : str.m_data);
	}

	void repeat(char c, size_t amount);

	void lower();
	void upper();

	void trim_lead();
	void trim_trail();
	void trim();

	size_t index_of(const char c, size_t offset = 0);

	[[nodiscard("Use the vector dumbass")]]
	std::vector<String> split(char delim) const;

	void erase(size_t index);
	void insert(size_t index, const char c);

	String slice(size_t start, size_t end) const;

	template<typename T>
	static String join(const std::vector<T>& vec, const char* symbol = "")
    {
        std::stringstream result;

        int i = 0;
        size_t size = vec.size();

        for (const auto& element : vec)
        {
            i++;
            result << element;

            if (i != size)
                result << symbol;
        }

        return result.str().c_str();
    }

private:
	char*  m_data;
	size_t m_size;
	size_t m_capacity;

	inline char& get_element(size_t index) const;

	template<typename T>
	inline bool cmp(T& str) const;

	inline void reassign(const char* str);

	inline void grow();

};