#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../include/traiettoria.hpp"

#include <iostream>

#include "doctest.h"

using pT = particleSimulator::posTypes;
namespace ps = particleSimulator;

TEST_CASE("Test del calcolo delle traiettorie") {
  // dati del biliardo di esembm::pi<float>()o
  const int r1 = 10;
  const int r2 = 5;
  const int l = 100;

  const particleSimulator::path biliardo{
      r1, r2, l};  // oggetto di calcolo della traiettoria

  // particelle
  particleSimulator::particle p0{{0, 0}, std::atanf(1)};
  particleSimulator::particle p1{{0, -8}, particleSimulator::arctan(4, 3)}; 
  particleSimulator::particle p2{{0, 0}, std::atanf(0)};
  particleSimulator::particle p3{{22, 8.92}, particleSimulator::arctan(-1, -5)};

  // Calcolo orientazione particella
  SUBCASE("Test calcolo orientazione traiettoria") {
    CHECK(biliardo.getHitDirection(p0.theta) ==
          particleSimulator::vecOrientation::Right);
    CHECK(biliardo.getHitDirection(p1.theta) ==
          particleSimulator::vecOrientation::Right);
    CHECK(biliardo.getHitDirection(p2.theta) ==
          particleSimulator::vecOrientation::Right);
    CHECK(biliardo.getHitDirection(p3.theta) ==
          particleSimulator::vecOrientation::Left);
  }

  // calcolo intersezioni
  const Eigen::Vector2f int0 = biliardo(p0).point;
  const Eigen::Vector2f int1 = biliardo(p1).point;
  const Eigen::Vector2f int2 = biliardo(p2).point;
  const Eigen::Vector2f int3 = biliardo(p3).point;

  SUBCASE("Test collisioni dopo il lancio") {
    CHECK(biliardo.getLocationType(int0) == pT::Inside);
    CHECK(biliardo.getLocationType(int1) == pT::Inside);
    CHECK(biliardo.getLocationType(int2) == pT::Escaped);
    CHECK(biliardo.getLocationType(int3) == pT::BackHit);
  }

  // test punto di intersezione
  SUBCASE("Test punto di intersezione") {
    // particella 0
    CHECK(int0.x() == doctest::Approx(9.5238095));
    CHECK(int0.y() == doctest::Approx(9.5238095));
    //particella 1
    CHECK(int1.x()==doctest::Approx(13.0119));
    CHECK(int1.y()==doctest::Approx(9.3493));
    //particella 2
    CHECK(int2.x()==doctest::Approx(100));
    CHECK(int2.y()==doctest::Approx(0));
   //particella 3
   CHECK(int3.x()==doctest::Approx(0));
   CHECK(int3.y()==doctest::Approx(4.520034));
  }

  // calcolo angoli riflessi
  SUBCASE("Test riflessione") {
    biliardo.reflect(p0);

    CHECK(p0.theta == doctest::Approx(5.39786)); //angolo della nuova traiettoria

    CHECK(biliardo.getHitDirection(p0.theta) ==
          particleSimulator::vecOrientation::Right); //nuova orientazione

    //test punto di urto 2
    biliardo.reflect(p0);

    CHECK(p0.pos.x()==doctest::Approx(24.488321));
    CHECK(p0.pos.y()==doctest::Approx(-8.7755799));
  }
}

TEST_CASE("Test dei casi critici") {  // casi che hanno o che potenzialmente
                                      // generano bachi
  const float r1{400};
  const float r2{200};
  const float l{700};

  const ps::path biliardo{r1, r2, l};
  const ps::simulation s{r1, r2, l, 200};

  SUBCASE("Test dei colpi ai bordi di uscita") {
    ps::particle p0{{0, 0}, 0.2793559};  // sfiora l'ultimo punto del bordo
    ps::particle p1{{0, 0},
                    0.2785637};  // colpisce il bordo sup poco prima della fine
    ps::particle p2{{0, 0}, 0.2793559};  // come p2 ma un po' più indietro
    ps::particle p3{{300, -314.2857142857}, 1.9684243};  // colpo all'angolo sup
    ps::particle p4{{300, -314.2857142857},
                    1.9683743};  // colpo vicino all'angolo sup
    ps::particle p5{{300, -314.2857142857}, 1.9684238};  // colpo all'angolo sup
    ps::particle p6{{300, -314.2857142857}, 1.9684293};  // colpo all'angolo sup
    ps::particle p7{{300, -314.2857142857}, 1.9684793};  // colpo all'angolo sup

    CHECK(biliardo.getHitDirection(p0.theta) == ps::vecOrientation::Right);
    CHECK(biliardo.getHitDirection(p1.theta) == ps::vecOrientation::Right);
    CHECK(biliardo.getHitDirection(p2.theta) == ps::vecOrientation::Right);
    CHECK(biliardo.getHitDirection(p3.theta) == ps::vecOrientation::Left);
    CHECK(biliardo.getHitDirection(p4.theta) == ps::vecOrientation::Left);
    CHECK(biliardo.getHitDirection(p5.theta) == ps::vecOrientation::Left);
    CHECK(biliardo.getHitDirection(p6.theta) == ps::vecOrientation::Left);
    CHECK(biliardo.getHitDirection(p7.theta) == ps::vecOrientation::Left);

    biliardo.reflect(p0);
    biliardo.reflect(p1);
    biliardo.reflect(p2);  // esegui le simulazioni
    biliardo.reflect(p3);
    biliardo.reflect(p4);
    biliardo.reflect(p5);
    biliardo.reflect(p6);
    biliardo.reflect(p7);

    // controllo stato finale
    CHECK(biliardo.getHitDirection(p0.theta) == ps::vecOrientation::Right);
    CHECK(biliardo.getHitDirection(p1.theta) == ps::vecOrientation::Right);
    CHECK(biliardo.getHitDirection(p2.theta) == ps::vecOrientation::Right);
    CHECK(biliardo.getHitDirection(p3.theta) == ps::vecOrientation::Left);
    CHECK(biliardo.getHitDirection(p4.theta) == ps::vecOrientation::Left);
    CHECK(biliardo.getHitDirection(p5.theta) == ps::vecOrientation::Left);
    CHECK(biliardo.getHitDirection(p6.theta) == ps::vecOrientation::Left);
    CHECK(biliardo.getHitDirection(p7.theta) == ps::vecOrientation::Left);
  }

  SUBCASE("Test della riflessione di un caso particolare") {
    ps::particle p{{0, -91.5416}, 5.82604};  // particella

    // direzione iniziale
    CHECK(biliardo.getHitDirection(p.theta)==ps::vecOrientation::Right);

    // sequenza delle posizioni occupate
    const std::vector<ps::particle> v = s.getSequence(p, 200);

    for (auto const& value : v) {  //controllo posizioni occupate
      std::cout<<value.pos.x()<<"\n";
    }

    // controllo posizioni
    // ultima: punto di uscita
    CHECK(v.at(2).pos.x() == doctest::Approx(l));
    // penultima: poco prima del punto di uscita
    CHECK(v.at(1).pos.x() < doctest::Approx(l));
    CHECK(v.size()==3);

    CHECK(biliardo.getLocationType(v.at(2).pos)==ps::posTypes::Escaped);
  }

  SUBCASE("Test della riflessione di un caso particolare") {
    particleSimulator::particle p{{0, 97.13790130615}, 5.6391};

    const std::vector<ps::particle> v = s.getSequence(p, 200);
    // inserire test delle posizioni occupate nella sequenza
    const std::array<Eigen::Vector2f,4> sequence{{{479.5721045,-262.97901},{663.275102,210.493},{579.317,-234.481},{0,396.056}}}; //aggiungere sequenza
    
    auto it{sequence.cbegin()}; //iteratore const per array

    for (auto const& value : v) {
      CHECK(value.pos.x() == doctest::Approx(it->x()));
      CHECK(value.pos.y() == doctest::Approx(it->y()));
      ++it;
    }
  }
}