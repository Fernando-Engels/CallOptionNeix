// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
#include <ConsoleApplication1/ConsoleApplication1/TestClass.h>
using namespace std;


  //Class with all the calculations
    class CallMath {
    private:
        std::vector<std::vector<std::string>> data;

    public:
        CallMath(std::vector<std::vector<std::string>> dataPass) {
            data = dataPass;
        }
        //calculate the averages between two columns of data
        std::vector<double> calculateAveragescols(int col1, int col2) {
            std::vector<double> averages;
            double total = 0.0;
            int count = 0;

            for (const auto& row : data) {
                if (row.size() > col1 && row.size() > col2) {
                    total = 0.0;
                    count = 0;

                    if (row[col1] != "\\N") {
                        total += std::stod(row[col1]);
                        count++;
                    }

                    if (row[col2] != "\\N") {
                        total += std::stod(row[col2]);
                        count++;
                    }

                    double average = (total / count);
                    averages.push_back(average);
                }
            }

            return averages;
        }
        //calculates the mean of a vector
        double calculateMean(std::vector<double> vector, int n) {

            double sum = 0.0;
            for (int i = 0; i < n; ++i) {
                sum += vector[i];
            }
            return sum / (n);

        }
        //calculates the volatility iterating through the call prices by using Black Scholes formula till it matches the desire call value
        //if it doesnt find a posible of volatility it assign the value 0
        std::vector<double> calculateVolatility(double seed, std::vector<double> timeVec, double risk_freeinterest) {
            std::vector<double> call_price;
            call_price = calculateAveragescols(3, 4);
            double strike_price = std::stod(data[0][1]);
            std::vector<double>  spot_price = calculateAveragescols(5, 6);
            double d1;
            double call_calculated;
            double d2;
            double dif;
            double dif_threshold = 1;
            double seed_correction = 0.01;
            std::vector<double> volatility;
            boost::math::normal_distribution<> normal(0, 1);
            int n = spot_price.size();

            for (int i = 0; i < n; ++i) {
                while (1) {
                    d1 = (log(spot_price[i] / strike_price) + (risk_freeinterest + 0.5 * pow(seed, 2)) * timeVec[i]) / (seed * sqrt(timeVec[i]));
                    d2 = d1 - seed * sqrt(timeVec[i]);
                    call_calculated = boost::math::cdf(normal, d1) * spot_price[i] - boost::math::cdf(normal, d2) * strike_price * exp(-risk_freeinterest * timeVec[i]);
                    dif = std::abs(call_price[i] - call_calculated);
                    if (dif < dif_threshold) {
                        break;
                    }
                    else {
                        if (seed > seed_correction) {

                            if (call_calculated < call_price[i]) {
                                seed += seed_correction;
                            }

                            else {
                                seed -= seed_correction;
                            }
                        }
                        else {
                            seed = 0;
                            break;
                        }
                        
                    }
                }
                volatility.push_back(seed);
            }
            return volatility;
        }
        //calculate historic volatility
        std::vector<double> calculatevolatilityHistoric() {
            std::vector<double> active_price = calculateAveragescols(5, 6);
            std::vector<double> vol_hist;
            std::vector<double> mean;
            int r = active_price.size();
            for (int i = 0; i < r; ++i) {
                mean.push_back(calculateMean(active_price, i + 1));
            }
            for (int i = 0; i < r; ++i) {
                vol_hist.push_back(sqrt(pow(active_price[i] - mean[i], 2) / (i + 1)));

            }
            return vol_hist;
        };

    };






    int main() {
        std::string file_path = "C:\\Users\\ferna\\OneDrive\\Escritorio\\challenge_Neix\\Exp_Octubre.csv";//file path of the csv
        std::string expiration_time = "10/20/2023 16:00";// expiration time of the call option
        double risk_free = 0.01;//free risk rate anually 1% in this case 

        CallOptionRead obj_read(file_path, expiration_time);
        CallMath obj_math(obj_read.data);
        CallOptionPlot plot;
        double seed = 0.1;// seed for the iteration of volatility calculation

        std::vector<double> time_vec = obj_read.time_vec;
        std::vector<double> volatility;
        std::vector<double> volatility_hist;

        //calculate both volatility through Blacks Scholes and historically
        volatility = obj_math.calculateVolatility(seed, time_vec, risk_free);
        volatility_hist = obj_math.calculatevolatilityHistoric();
        plot.plotVolatility(volatility, volatility_hist, time_vec);//plot
        return 0;
        // Run program: Ctrl + F5 or Debug > Start Without Debugging menu
    // Debug program: F5 or Debug > Start Debugging menu

    // Tips for Getting Started: 
    //   1. Use the Solution Explorer window to add/manage files
    //   2. Use the Team Explorer window to connect to source control
    //   3. Use the Output window to see build output and other messages
    //   4. Use the Error List window to view errors
    //   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
    //   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
    }



