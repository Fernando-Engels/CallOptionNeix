#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <boost/math/distributions/normal.hpp>
#include <cmath>
using namespace std;
//In this file there are the classes to manage the csv and the class to plot
//class to open the csv and put the data into a variable, and calculate time till expire
class CallOptionRead {
public:
    std::vector<std::vector<std::string>> data;
    std::string expiration_time;
    std::vector <double> time_vec;
    double risk_freeinterest;


    //constructor
    CallOptionRead(const std::string& filePath, std::string expirationTime) {
        managecsv(filePath);
        expiration_time = expirationTime;
        time_vec = calculateTime();
    }
    //open csv and manage it into variable data
    void managecsv(const std::string& filePath) {
        std::ifstream file(filePath); // open the csv file
        std::string line;
        if (file.is_open()) {
            // Read each line in the file
            std::getline(file, line);
            while (std::getline(file, line)) {

                std::replace(line.begin(), line.end(), ',', '.'); // Replace commas with dots in the line
                std::istringstream linestream(line);
                std::string cell;
                std::vector<std::string> row; // Vector to store cells of a row

                // Read and store each cell separated by ';'
                while (std::getline(linestream, cell, ';')) {
                    row.push_back(cell); // Store cell in the row vector
                }
                data.push_back(row); // Store row in the data vector
            }
            file.close(); // Close the file
        }
        else {
            std::cout << "Unable to open file" << std::endl;
        }

    }

    // Calculate time to expire time in years
    std::vector<double> calculateTime() {
        int n;
        std::tm tm1 = {};
        std::tm tm2 = {};
        std::chrono::system_clock::time_point tp1;
        std::chrono::system_clock::time_point tp2;
        std::chrono::duration<double> duration;
        std::vector<double> time_differences;
        double num_secondsinyear = 31536000.0;

        n = data.size();
        std::string timeStr1;
        for (int i = 0; i < n; ++i) {
            timeStr1 = data[i][7];
            std::istringstream ss1(timeStr1);
            ss1 >> std::get_time(&tm1, "%m/%d/%Y %H:%M");

            std::istringstream ss2(expiration_time);
            ss2 >> std::get_time(&tm2, "%m/%d/%Y %H:%M");

            if (ss1.fail() || ss2.fail()) {
                std::cout << "Failed to parse time string." << std::endl;
                continue; // Skip to the next row if parsing fails
            }

            tp1 = std::chrono::system_clock::from_time_t(std::mktime(&tm1));
            tp2 = std::chrono::system_clock::from_time_t(std::mktime(&tm2));
            duration = tp2 - tp1;
            time_differences.push_back(duration.count() / num_secondsinyear);
        }

        return time_differences;
    };
};

//class to plot the vectors, both volatilities as a function of time
class CallOptionPlot {
public:
    void plotVolatility(std::vector<double> vectorY1, std::vector<double> vectorY2, std::vector<double> vectorX) {
        if (vectorX.size() != vectorY1.size() || vectorX.size() != vectorY2.size()) {
            std::cerr << "Error: Vector sizes do not match!" << std::endl;
            return;
        }

        // Writing data to a text file
        std::ofstream dataFile("plot_data.txt");
        if (!dataFile.is_open()) {
            std::cerr << "Error: Unable to open data file!" << std::endl;
            return;
        }
        for (size_t i = 0; i < vectorX.size(); ++i) {
            dataFile << vectorX[i] << " " << vectorY1[i] << " " << vectorY2[i] << std::endl; // Writing data in time vola culi format
        }
        dataFile.close();

        // Creating GNUPLOT script
        std::ofstream scriptFile("plot_script.plt");
        if (!scriptFile.is_open()) {
            std::cerr << "Error: Unable to open plot script file!" << std::endl;
            return;
        }
        // Writing GNUPLOT commands to the script file
        scriptFile << "set terminal wxt persist\n";  // Set output terminal to interactive window and keep it open
        scriptFile << "set xlabel 'Time in years until expire'\n";  // Set x-label
        scriptFile << "set ylabel 'Volatility'\n";  // Set y-label
        scriptFile << "set xrange [*:*] reverse\n";  // Set x-axis range in reverse order
        scriptFile << "plot 'plot_data.txt' using 1:2 with lines title 'volatility from Black Scholes', "
            "'plot_data.txt' using 1:3 with lines title 'historic volatility'\n";  // Plot data from file

        scriptFile.close();

        // Execute GNUPLOT with the generated script file
        system("gnuplot plot_script.plt");

    }
};
