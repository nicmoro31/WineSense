#ifndef SENSORE_H
#define SENSORE_H

#include <string> // Include the <string> header file
#include <vector> // Include the <vector> header file
#include <chrono>

class Autoclave;    // Forward declaration of Autoclave

class Sensore{
private:
    static unsigned int id;
    std::string tipo; 
protected:
    double sogliaMin;
    double sogliaMax;
    double nuovoDato;
    std::vector<std::chrono::system_clock::time_point> timestamps;
public:
    Sensore();
    Sensore(std::string tipo, double sogliaMin, double sogliaMax);
    static Sensore* create(std::string tipo, double sogliaMin, double sogliaMax, Autoclave* autoclave);
    virtual ~Sensore();
    virtual Sensore* clone() const = 0;
    bool operator==(const Sensore& other) const;
    bool operator!=(const Sensore& other) const;
    int getId();
    std::string getTipo() const;
    double getSogliaMinima() const;
    double getSogliaMassima() const;
    virtual std::vector<double> getDati() const = 0;
    virtual std::vector<std::chrono::system_clock::time_point> getTimestamps() const;
    virtual void setDati(const std::vector<double>&, const std::vector<std::chrono::system_clock::time_point>&) = 0;
    void impostaSoglia(double, double);
    virtual void controllaAnomalia() = 0;

    virtual std::pair<double, double> getLimiti() const = 0;
    virtual std::string getUnitaMisura() const = 0;
    
};

#endif // SENSORE_H
