#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
using namespace std;

class spalatorie {
    static int cant_fixa; //cantitatea fixa de detergent de la care se calculeaza celelalte
    static int timp_de_spalare; //timpul de spalare standard'
    static spalatorie* singleton;
    spalatorie() {};
public:
    static spalatorie* make_spalatorie() {
        if (!singleton) {
            singleton=new spalatorie;
            return singleton;
        }
        return singleton;
    }
    static int get_cant() {return spalatorie::cant_fixa;}
    static int get_timp() {return spalatorie::timp_de_spalare;}
};


spalatorie* spalatorie::singleton=0;
int spalatorie::cant_fixa=10;
int spalatorie::timp_de_spalare=100;

class haina {
protected:
   // int id;
    int greutate;
    bool culoare; //0=desch, 1=inchisa
    bool grea;//0=nu, 1=da
    haina(int greutate_, bool culoare_, bool grea_) {
        greutate=greutate_;
        culoare=culoare_;
        grea=grea_;
    }
public:

    static haina* make_haina(int alegere, int greutate_, bool culoare_, bool grea_); //1=blugi, 2=camasi etc;
    virtual int detergent_folosit() {};
    virtual int timp_calcat() {};// virtual pt ca difera! de la porodus la produs
    int timp_spalare() {return spalatorie::get_timp();};
    int get_greutate() {return greutate;}
    virtual int plasare() {};
//in functie de tipul hainei, o pun intr-o masina de spalat
//in total e un vector cu 6 masini de spalat, pentru ca sunt 6 combinatii de haine - deschise, usoare, la temp.ridicata, scazuta etc.
};

class masina{
protected:
    int greutate_curenta;
    int itemi_curent;
    static int capacitate;
    static int itemMax;
    int nr_masina;
    vector<haina*> haine;
    bool ocupat; //0-masina goala, 1-masina ocupata
public:
    int get_nr_masina() {return masina::nr_masina;}
    virtual int actiune(haina *h){};
};

int masina::capacitate=80;
int masina::itemMax=20;

class masina_spalat : protected masina{
private:
    vector<haina*> haine;
public:
   /* virtual int actiune(haina *h){
        int greutate_curenta = h->get_greutate();
        if (greutate_curenta>=capacitate/2){
            greutate_curenta = 0;
            haine.pop_back();
        }
    }*/

    virtual int actiune(int i){
    if (greutate_curenta + haine[i]->get_greutate() >=capacitate/2){
            greutate_curenta = 0;
           do {haine.erase(haine.begin()+i);}
                while (haine.size());
        } else
        greutate_curenta += haine[i]->get_greutate();
}
};


class masina_uscat : protected masina {
private:
    vector<haina*> haine;
public:
    virtual int actiune(int i){
    if (itemi_curent + 1 >= itemMax/2){
           itemi_curent = 0;
           do {haine.erase(haine.begin()+i);}
                while (haine.size());
        } else
        itemi_curent ++;
}
};

class pantaloni: public haina {
public:
    pantaloni(int greutate_, bool culoare_, bool grea_):haina(greutate_,culoare_,grea_) {};
    int detergent_folosit() {
        if (culoare) return spalatorie::get_cant()*2;
        else return 100*greutate;
    }
    int timp_calcat(){
        return 90*greutate;}
    int plasare(){
        if (culoare) return 1;
            else return 2;
        }
};

class camasa: public haina {
public:
    camasa (int greutate_, bool culoare_, bool grea_):haina(greutate_,culoare_,grea_) {};
    int detergent_folosit() {
          return spalatorie::get_cant();
    }
    int timp_calcat(){
        return 120*greutate;}
    int plasare(){
        if (culoare) return 1;
            else return 2;
        }
};

class rochie: public haina {
public:
    rochie (int greutate_, bool culoare_, bool grea_):haina(greutate_,culoare_,grea_) {};
    int detergent_folosit() {
       return spalatorie::get_cant();
    }
    int plasare(){
        if (culoare) return 1;
            else return 2;
        }
};

class palton: public haina {
public:
    palton (int greutate_, bool culoare_, bool grea_):haina(greutate_,culoare_,grea_) {};
    int detergent_folosit() {
       return 100*greutate;
    }
    int plasare(){
        if (culoare) return 3;
            else return 4;
        }
};

class geaca: public haina {
public:
    geaca (int greutate_, bool culoare_, bool grea_):haina(greutate_,culoare_,grea_) {};
    int detergent_folosit() {
       return 100*greutate;
    }
     int plasare(){
        if (culoare) return 5;
            else return 6;
        }
};

class costum: public haina {
public:
    costum (int greutate_, bool culoare_, bool grea_):haina(greutate_,culoare_,grea_) {};
    int detergent_folosit() {
       return 100*greutate;
    }
    int timp_calcat(){
        return 150*greutate;}
     int plasare(){
        if (culoare) return 3;
            else return 4;
        } //plasam costumele cu paltoanele
};

haina* haina::make_haina(int alegere, int greutate_, bool culoare_, bool grea_) {
        haina* ret;
        if (alegere==1) {
            ret=new pantaloni ( greutate_, culoare_, grea_);
            return ret;
        };
        if (alegere==2){
            ret=new camasa ( greutate_, culoare_, grea_);
            return ret;
        };
        if (alegere==3){
            ret=new rochie ( greutate_, culoare_, grea_);
            return ret;
        };
        if (alegere==4){
            ret=new palton ( greutate_, culoare_, grea_);
            return ret;
        };
        if (alegere==5){
            ret=new geaca ( greutate_, culoare_, grea_);
            return ret;
        };
        if (alegere==6){
            ret=new costum ( greutate_, culoare_, grea_);
            return ret;
        };
    }

class client {
    vector<haina*> haine;
    int id_client;

public:
    client() {};
    friend istream& operator>>(istream& in, client& cl) {
        cout<<"Nr haine:";
        int nr_haine;
        in>>nr_haine;
        cout<<endl;
        for (int i=0; i<nr_haine; i++) {
            int alegere, greutate;
            bool cul, grea;
            in>>alegere>>greutate>>cul>>grea;
            cl.haine.push_back(haina::make_haina(alegere, greutate, cul, grea));
        }
    }
    friend ostream& operator<<(ostream& o, client& cl) {
        int total_detergent=0, total_timp=0;
        for (int i=0; i<cl.haine.size(); i++) {
            total_detergent+=cl.haine[i]->detergent_folosit();
            total_timp+=cl.haine[i]->timp_spalare();
        }
        o<<"Timp spalare: "<<total_timp<<endl<<"Total detergent folosit: "<<total_detergent;
        return o;
    }
};


vector<client> clienti;
vector<masina> masini;
int main(){
    spalatorie* spal=spalatorie::make_spalatorie();
    int nrClienti;
    cout << "Cati clienti stau la coada astazi?";
    cin >> nrClienti;
    for(int i=0; i<nrClienti; i++) {
        client cl;
        clienti.push_back(cl);
        cin>>clienti[i];
        cout<<clienti[i];
    }
}
