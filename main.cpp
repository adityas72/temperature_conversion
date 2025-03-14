#include <iostream>
#include <unordered_map>
#include <functional>
#include <limits>

using ConversionFunction = std::function<double(double)>;

class TemperatureConverter {
private:
    std::unordered_map<std::string, std::unordered_map<std::string, ConversionFunction>> conversions;

    // Validate Kelvin value (Kelvin cannot be below absolute zero)
    void validateKelvin(double kelvin) {
        if (kelvin < 0) {
            throw std::invalid_argument("Kelvin temperature cannot be below absolute zero (0K).");
        }
    }

public:
    TemperatureConverter() {
        // Conversion formulas
        conversions["C"]["F"] = [](double c) { return (c * 9 / 5) + 32; };   // Celsius to Fahrenheit
        conversions["C"]["K"] = [](double c) { return c + 273.15; };         // Celsius to Kelvin
        conversions["F"]["C"] = [](double f) { return (f - 32) * 5 / 9; };   // Fahrenheit to Celsius
        conversions["F"]["K"] = [](double f) { return (f - 32) * 5 / 9 + 273.15; }; // Fahrenheit to Kelvin
        conversions["K"]["C"] = [this](double k) { validateKelvin(k); return k - 273.15; }; // Kelvin to Celsius
        conversions["K"]["F"] = [this](double k) { validateKelvin(k); return (k - 273.15) * 9 / 5 + 32; }; // Kelvin to Fahrenheit
    }

    double convert(double value, const std::string& from, const std::string& to) {
        if (from == to) return value;  // No conversion needed
        if (from == "K") validateKelvin(value);  // Validate Kelvin input

        try {
            return conversions.at(from).at(to)(value);
        } catch (const std::out_of_range&) {
            throw std::invalid_argument("Unsupported conversion.");
        }
    }
};

int main() {
    TemperatureConverter converter;
    int choice;
    double temp;
    
    std::unordered_map<int, std::string> scales = {
        {1, "C"}, {2, "F"}, {3, "K"}
    };

    while (true) {
        std::cout << "\n🌡 Temperature Conversion Menu:\n";
        std::cout << "1. Celsius to Fahrenheit/Kelvin\n";
        std::cout << "2. Fahrenheit to Celsius/Kelvin\n";
        std::cout << "3. Kelvin to Celsius/Fahrenheit\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (choice == 4) break;

        try {
            std::string from = scales.at(choice);
            std::cout << "Enter temperature in " << from << ": ";
            if (!(std::cin >> temp)) {
                throw std::invalid_argument("Invalid temperature value.");
            }

            for (const auto& [key, to] : scales) {
                if (key != choice) {
                    std::cout << " -> " << to << ": " << converter.convert(temp, from, to) << "\n";
                }
            }
        } catch (const std::out_of_range&) {
            std::cout << "Invalid choice. Please try again.\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    return 0;
}
