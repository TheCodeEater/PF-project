#include <string>
#include <fstream>
#include <stdexcept>
#include "TCanvas.h"
#include "TH1F.h"
#include "TMath.h"
#include "TStyle.h"

void analyse_exit(std::string const& filename){
    TCanvas* c=new TCanvas{"c1","Distribuzione delle coordinate di uscita delle particelle",1000,600};

    gStyle->SetOptStat(110111110); //kurtosis, skewness, underflow, overflow, rms, mean, entries
    c->Divide(2,1);

    ifstream f{filename};

    if(!f.is_open()){
        throw std::runtime_error("File non trovato!");
    }

    Float_t r2{}; //lettura parametro r2 per impostare il range dell'istogramma delle altezze di uscita
    f>>r2;

    TH1F* theta_g=new TH1F{"exit_angles","Angoli di uscita; Angolo (rad); Occorrenze",100,0,2*TMath::Pi()+0.01};
    TH1F* y_g=new TH1F{"exit_heights","Altezza di uscita; Altezza (px); Occorrenze",500,-410,410};

    while(f.good()){ //leggi i dati finche' possibile
        Float_t value_angle{};
        Float_t value_y{};
        f>>value_y>>value_angle;

        theta_g->Fill(value_angle);
        y_g->Fill(value_y);
    }
    //pad 1
    c->cd(1);
    //stile 
    theta_g->SetFillColor(kBlue);
    //imposta label con gli angoli in radianti
    //disegna
    theta_g->Draw("HIST");

    //pad 2
    c->cd(2);
    //imposta colore 
    y_g->SetFillColor(kBlue);
    //disegna
    y_g->Draw("HIST");

}