#ifndef	UTILS_HPP
# define UTILS_HPP

# include <unistd.h>
# include <string>
# include <iostream>
# include <cstdlib>
# include <vector>
# include <algorithm>
# include <cctype>

std::vector<std::string>	split(const std::string& str, const std::string& delimiter);
std::vector<std::string>	vecToLowerCase(std::vector<std::string> vec);
std::string                 toUpperCase(const std::string& input);
std::string 				toLowerCase( std::string &str);
std::string 				removeCharacter(const std::string& str, char target);
void						removeElementFromVector(std::vector<int> &vector, int element);
void						ft_bzero(void *s, size_t n);
void						handlesginal(int sig);
void						removeWhitespace(std::string& str);
bool						IsDigit(std::string str);
bool						isNickValid(const std::string &nick);
bool						containsSubstring(const std::string& haystack, const std::string& needle);
bool						isInside(std::vector<std::string> vec, std::string str);
int							printMessage(std::string message, std::string color);

#endif
