#pragma once

#include <cstring>
#include "asf.h"

namespace kronos {
    //! \class String Class
    class String {
    public:
        //! \brief Empty Constructor for String Class
        String() {
            m_Length = 0;
            // Creates a new string with the same length as str
            char* newStr = new char[m_Length + 1];
            // Creates a string of length 0 with a null character
            memset(newStr, '\0', m_Length + 1);
            // Stores the new string value.
            m_String = newStr;
        }

        //! \brief Copy Constructor for String Class
        //! \param str is the string to be copied
        String(const char* str) {
            m_Length = 0;
            // Iterates through str to find the length.
            while (str[m_Length] != '\0')
                m_Length++;
            // Creates a new string with the same length as str
            char* newStr = new char[m_Length + 1];
            // Copies the values of str to the new String
            memcpy(newStr, str, m_Length + 1);
            // Stores the new string value.
            m_String = newStr;
        }

        //! \brief Copy Constructor for String class
        //! \param from is the string to be copied
        String(const String& from) : m_Length(from.m_Length) {
            // Create a new string with the same length
            char* str = new char[m_Length + 1];
            //Copy contents of the reference to the new string
            memcpy(str, from.m_String, m_Length + 1);
            // Store the value of the new string.
            m_String = str;
        }

        //! \brief Default Destructor for String class
        ~String() {
            //Checks if the String is Empty
            if (m_String == nullptr)
                return;
            //Deletes the String Contents
            delete[] m_String;
            //Removes the reference to the memory address
            m_String = nullptr;
        }

        //! \brief Function to return String as Character array
        //! \return character array of the String
        const char* Ptr() const { return m_String; }

        //! \brief Getter Function for the Length of the String
        //! \return
        size_t Size() const { return m_Length; }

        //! \brief Function to Hash the String
        //! \return Returns the Hash Value of the String.
        size_t Hash() const {
            // predefined hash value
            size_t hash = 5381;
            //Iterates through the string and updates the Hash value based on the characters
            for (size_t i = 0; i < m_Length; i++)
                hash = hash * 33 + m_String[i];
            return hash;
        }

        //! \brief Function to find the address of a String
        //! \param str is the string who's address need to be found
        //! \return Start of the Address
        size_t Find(const char* str) const {
            //
            char* ptr = strstr(m_String, str);
            if (ptr == nullptr)
                return UINT32_MAX;

            return ptr - m_String;
        }

        //! \brief Function to create a Substring from an index till the end
        //! \param start is the index to begin the subset
        //! \return Returns the Substring of the string
        String Substring(size_t start) const {
            return Substring(start, m_Length);
        }

        //! \brief Function to create a Substring
        //! \param start is the index to begin the subset
        //! \param end is the index to end the subset
        //! \return Returns the Substring of the string
        String Substring(size_t start, size_t end) const {
            //Checks if the indices are valid.
            if (start >= m_Length || end > m_Length || end <= start)
                return "";
            // Returns the subset
            return String(*this, start, end);
        }

        //! \brief Fucntion to concatenate two strings
        //! \param first is one of the strings to be concatenated
        //! \param second is one of the strings to be concatenated
        //! \return Concatenated string made from first and second
        static String Concatenate(const String& first, const String& second) {
            // Creates a new character array to hold both strings
            char* newStr = new char[first.m_Length + second.m_Length + 1];
            //Copies the first string
            memcpy(newStr, first.m_String, first.m_Length);
            //Copies the second string at the end of the first string
            memcpy(newStr + first.m_Length, second.m_String, second.m_Length);
            //Adds a Ending Character for the array
            newStr[first.m_Length + second.m_Length] = '\0';
            //Creates a string from the character array.
            String result(newStr);
            // Deletes the redundant character array
            delete[] newStr;
            //Returns the new String
            return result;
        }

        //! \brief Function to Concatenate strings by using the '+' Operator
        //! \param left is the first string to be concatenated
        //! \param right is the second string to be concatenated
        //! \return Concatenated string
        friend String operator+(const String& left, const String& right) {
            //Calls the Concatenate Function
            return String::Concatenate(left, right);
        }

        //! \brief Function to concatenate a second string with the first one
        //! \param right String to be concatenated to the existing string
        //! \return
        String& operator+=(const String& right) {
            *this = Concatenate(*this, right);
            return *this;
        }

        //! \brief Assignment Operator Overload
        //! \param other is the string which is going to be assigned
        //! \return The new string after the assignment
        String& operator=(const String& other) {
            if (this == &other)
                return *this;

            // Delete previous buffer
            delete[] m_String;

            // Allocate new buffer
            char* newStr = new char[other.m_Length + 1];
            m_Length = other.m_Length;
            memcpy(newStr, other.m_String, other.m_Length + 1);
            m_String = newStr;

            return *this;
        }

        //! \brief Friend Function to check if two strings are equal
        //! \param lhs is one of the strings to be checked
        //! \param rhs is one of the strings to be checked
        //! \return True if the strings are equal, false otherwise
        friend bool operator==(const String& lhs, const String& rhs) {
            //Checks if the length is the same
            if (lhs.m_Length != rhs.m_Length) {
                return false;
            }
            //Iterates through and compares the characters
            for (size_t i = 0; i < lhs.m_Length + 1; i++) {
                if (lhs.m_String[i] != rhs.m_String[i]) {
                    // Strings are not equal
                    return false;
                }
            }
            //Strings are equal
            return true;
        }

        //! \brief Function to check if both strings are not the same
        //! \param lhs is one of the strings to be checked
        //! \param rhs is one of the strings to be checked
        //! \return True if the strings are not equal, false otherwise
        friend bool operator!=(const String& lhs, const String& rhs) {
            return !(lhs == rhs);
        }

        //! \brief Function to find the character in a particular index
        //! \param idx is the index of the character
        //! \return Character in a particular index
        char operator[](size_t idx) {
            return m_String[idx];
        }

    private:
        //! \brief Parametrized Constructor for String Class
        //! \param from is the string to be copied
        //! \param start is the beginning index of the string
        //! \param end is the ending index of the string
        String(const String& from, size_t start, size_t end) {
            char* str = new char[end - start + 1];
            memcpy(str, from.m_String + start, end - start);
            str[end - start] = '\0';
            m_String = str;
            m_Length = end - start;
        }

        //! Character Pointer for the String
        const char* m_String;
        //! Size of the String
        size_t m_Length;
    };

}