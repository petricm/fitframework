/**
 * @brief Header file with some useful utilities
 *
 * Can be included into any file/project, no extra implementation file necessary
 */

#ifndef MP_UTILITIES_H_
#define MP_UTILITIES_H_

#include <vector>
#ifdef __GXX_EXPERIMENTAL_CXX0X__
#include <tuple>
#endif
#include <utility>
#include <map>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <stdexcept>

namespace mp {

/**
 * @brief Returns a string of t if t overloads << operator
 *
 * @param t will be converted to a string
 * @return string of t
 */
template <class T>
inline std::string to_string (const T& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}

/**
 * @brief Returns string representation of var
 *
 * @param var boolean to be converted
 * @return "true" or "false"
 */
inline std::string bool_string(bool var) {
    if(var)
        return "true";
    else
        return "false";
}

/**
 * @brief Creates a std::string representation of std::vector<T> if T overloads << operator
 * @param vec vector to convert
 * @param newline true if newline should be used for each value
 * @return string of vec
 */
template <class T>
inline std::string vector_to_string(const std::vector<T> &vec, bool newline = true) {
    std::stringstream ss;
    for (size_t i = 0; i < vec.size(); ++i) {
        ss << "Vector[" << i << "] " << vec[i];
        if(newline)
            ss << "\n";
        else
            ss << " ";
    }
    return ss.str();
}

/**
 * @brief Creates a std::string representation of std::pair<T1,T2> if T1 and T2 overload << operator
 * @param pair pair to convert
 * @return string of pair
 */
template <class T1, class T2>
inline std::string pair_to_string(const std::pair<T1, T2> &pair) {
    std::stringstream ss;
    ss << "Pair[" << pair.first << "," << pair.second << "]";
    return ss.str();
}

#ifdef __GXX_EXPERIMENTAL_CXX0X__
template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<(I == sizeof...(Tp)), void>::type
stream_tuple(std::stringstream& /*ss*/, const std::tuple<Tp...>& /*t*/) {
}

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<(I < sizeof...(Tp)), void>::type
stream_tuple(std::stringstream& ss, const std::tuple<Tp...>& t) {
	ss << std::get<I>(t);
	if(I+1 < sizeof...(Tp))
		ss << ",";
	stream_tuple<I + 1, Tp...>(ss,t);
}

/**
 * @brief Creates a std::string representation of std::tuple<T...> if all T... overload << operator
 * @param tuple tuple
 * @return string of tuple
 */
template <typename ... Args>
inline std::string tuple_to_string(const std::tuple<Args...> &tuple) {
    std::stringstream ss;
    ss << "Tuple[";
    stream_tuple(ss,tuple);
    ss << "]";
    return ss.str();
}
#endif

/**
 * @brief Creates a std::string representation of std::map<T1,T2> if T1 and T2 overload << operator
 * @param map map to convert
 * @return string of map
 */
template <class T1, class T2>
inline std::string map_to_string(const std::map<T1, T2> &map) {
    std::stringstream ss;
    for(typename std::map<T1,T2>::const_iterator it = map.begin(); it != map.end(); ++it) {
    	ss << "Key[" << (*it).first << "] = " << (*it).second << "\n";
    }
    return ss.str();
}

/**
 * @brief Calculate mean of a given vector
 * @param vec values
 * @return mean
 */
template <class T>
inline double calc_mean(const std::vector<T> &vec){
	double sum = 0;
	for(unsigned int j = 0; j < vec.size();j++){
		sum += vec[j];
	}
	sum /= vec.size();
	return sum;
}

/**
 * @brief Calculate sigma of a given vector
 * @param vec values
 * @return sigma
 */
template <class T>
inline double calc_sigma(const std::vector<T> &vec){
	double sum = 0;
	double mean = mp::calc_mean(vec);

	for(unsigned int j = 0; j < vec.size();j++){
		sum += (vec[j] - mean)*(vec[j] - mean);
	}
	sum /= vec.size()-1;
	return std::sqrt(sum);
}

/**
 * @brief Calculate weighted mean of a given vector
 * @param vec values
 * @param err weights
 * @return weighted mean
 */
template <class T, class S>
inline double calc_weighted_mean(const std::vector<T> &vec, const std::vector<S> &err){
	double sum = 0;
	double weight;
	double sumweight = 0;

	for(unsigned int j = 0; j < vec.size();j++){
		weight = 1.0/(err[j]*err[j]);
		sum += weight*vec[j];
		sumweight += weight;
	}
	sum /= sumweight;
	return sum;
}

/**
 * @brief Calculate weighted sigma of a given vector
 * @param vec values
 * @param err weights
 * @return weighted sigma
 */
template <class T, class S>
inline double calc_weighted_sigma(const std::vector<T> &vec, const std::vector<S> &err){
	double sum = 1;
	double weight;
	double sumweight = 0;

	for(unsigned int j = 0; j < vec.size();j++){
		weight = 1.0/(err[j]*err[j]);
		sumweight += weight;
	}
	sum /= sumweight;
	return std::sqrt(sum);
}

/**
 * @brief Computes efficiency and eff-error
 * @param n_sel selected events
 * @param n_all sum of selected and non-selected events
 * @return pair with efficiency and eff-error
 */
inline std::pair<double,double> calc_efficiency_and_error(double n_sel, double n_all) {
	double eff = n_sel/n_all;
	double eff_error = std::sqrt((n_sel*(n_all-n_sel))/(n_all*n_all*n_all));
	return std::make_pair(eff,eff_error);
}

/**
 * @brief Executes command cmd and in case of error, prints command and exists with EXIT_FAILURE code
 * @param cmd command to execute
 */
inline void execute_command(const std::string &cmd) {
	int err = system(cmd.c_str());
	if(err != 0) {
		std::cerr << "ERROR: Failed to execute command " << cmd << " with error code " << to_string(err) << std::endl;
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief Executes command cmd and in case of error, prints command and throws runtime_error if fails
 * @param cmd command to execute
 */
inline void execute_command_throw(const std::string &cmd) {
	int err = system(cmd.c_str());
	if(err != 0) {
		throw std::runtime_error("Failed to execute command "+cmd+" with error code "+to_string(err));
	}
}

} // namespace mp

#endif /* MP_UTILITIES_H_ */
