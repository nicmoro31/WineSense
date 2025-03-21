#ifndef SENSOREUMIDITA_H
#define SENSOREUMIDITA_H

#include <string>
#include <vector>


class SensoreUmidita: public Sensore{
private:
    std::vector<double> umidita;
public:
    SensoreUmidita(const std::string& tipo, double sogliaMin, double sogliaMax);
    ~SensoreUmidita();
    SensoreUmidita* clone() const;
    std::vector<double> getDati() const;
    void setDati(const std::vector<double>&, const std::vector<std::chrono::system_clock::time_point>&) override;
    void controllaAnomalia();
    std::pair<double, double> getLimiti() const;
    std::string getUnitaMisura() const;
};

#endif // SENSOREUMIDITA_H
