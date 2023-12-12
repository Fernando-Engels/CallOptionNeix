# Call Option Volatility Calculator

This program calculates both implicit and historical volatilities for call options using a provided CSV file containing relevant call option data. It leverages the Black-Scholes model for iterative implicit volatility computation and performs calculations to derive historical volatility.

## Features

- **Implicit Volatility Calculation**: Computes implicit volatility using the Black-Scholes model based on provided call option data.
- **Historical Volatility Calculation**: Derives historical volatility from stock price variations over time.
- **Comparison**: Compares calculated implicit volatility with derived historical volatility.

## Prerequisites

- C++ compiler (for compiling and running the program).
- CSV file containing call option data with columns: `description`, `strike price`, `kind`, `bid price`, `ask price`, `bid price asset`, `ask price asset`, and `time`.
- GNU plot

## Usage

### Input File Format

The CSV file should contain the following columns:

- `description`: Description of the asset.
- `strike price`: Strike price of the call option.
- `kind`: Call type.
- `bid price`: Bid price of the call option.
- `ask price`: Ask price of the call option.
- `bid price asset`: Bid price of the asset.
- `ask price asset`: Ask price of the asset.
- `time`: Time at which the stock price is recorded.

The program performs the following steps:

1. Reads the CSV file and stores its values.
2. Calculates the time till expiration for each call option.
3. Computes the average between bid and ask prices of both call options and the asset.
4. Determines the implicit volatility via the Black-Scholes formula by iteratively adjusting the volatility until the calculated call option price matches the Black-Scholes formula's price. This process involves initial guessing and adjustments based on the calculated call option prices.
5. Calculates historical volatility as the standard deviation of a set of points.
6. Plots both volatilities.

The figure initially shows significant disparity due to insufficient data to accurately represent historical volatility. With fewer data points, volatility tends to be high until more data stabilizes the calculation. As the process continues, both volatilities follow similar trends.

Expectedly, the implied volatility tends to be higher than historical volatility. This is due to the limited downside risk for the option's buyer compared to the unlimited downside risk for the seller.

Approaching the expiration date, the implied volatility tends towards very low values. This decline occurs because implied volatility serves as a predictive measure, representing future volatility. As expiration nears, the volatility approaches zero, reflecting the absence of future predictions.
