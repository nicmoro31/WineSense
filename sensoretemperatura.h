#ifndef SENSORETEMPERATURA_H
#define SENSORETEMPERATURA_H


#include <string> // Include the <string> header file
#include <vector> 

class SensoreTemperatura : public Sensore{
private:
    std::vector<double> temperatura;
public:
    SensoreTemperatura(const std::string& tipo, double sogliaMin, double sogliaMax);
    ~SensoreTemperatura();
    SensoreTemperatura* clone() const;
    std::vector<double> getDati() const;
    void setDati(const std::vector<double>&, const std::vector<std::chrono::system_clock::time_point>&) override;
    void controllaAnomalia();
    std::pair<double, double> getLimiti() const;
    std::string getUnitaMisura() const;
};

#endif // SENSORETEMPERATURA_H
