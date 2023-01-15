#include <iostream>
#include <cmath>
using namespace std;

class CCoord {
    public:
    double x, y, h, x2, y2, h2;
    public:
    CCoord(double x_, double y_, double h_, double x2_, double y2_, double h2_){  // Конструктор задает 2 пеленгатора 
        x = x_;
        y = y_;
        h = h_;
        x2 = x2_;
        y2 = y2_; 
        h2 = h2_;
    }
    void f_Print() { 
        cout << endl;
        cout << "x: " << x << endl;
        cout << "y: " << y << endl;
        cout << "h: " << h << endl;
        cout << "-------" << endl;
        cout << "x2: " << x2 << endl;
        cout << "y2: " << y2 << endl;
        cout << "h2: " << h2 << endl;
    }
};

class CPelengs{ 
    public:
    double E1, B1, E2, B2;
    public:
    CPelengs(double a_E1, double a_B1, double a_E2, double a_B2){ // Вводятся азимуты и углы места
        E1 = a_E1;
        B1 = a_B1;
        E2 = a_E2;
        B2 = a_B2;
    }
    void f_Print(){
        cout << endl;
        cout << "E1 - " << E1 << endl;
        cout << "B1 - " << B1 << endl;
        cout << "-------" << endl;
        cout << "E2 - " << E2 << endl;
        cout << "B2 - " << B2 << endl;
        cout << endl;
     }

};

class CStep1 {
    public: 
    double cos_ax, cos_ay, cos_ah, cos_bx, cos_by, cos_bh;
    CStep1(CPelengs cpeleng){  //задаются направляющие косинусы
        cos_ax = cos(cpeleng.E1) * cos(cpeleng.B1); //
        cos_ay = cos(cpeleng.E1) * sin(cpeleng.B1); // косинусы первого пеленга
        cos_ah = sin(cpeleng.E1);             //

        cos_bx = cos(cpeleng.E2) * cos(cpeleng.B2); //
        cos_by = cos(cpeleng.E2) * sin(cpeleng.B2); // косинусы второго пеленга
        cos_bh = sin(cpeleng.E2);             
    }
    
}; 

class CStep2{   
    public:
    double a1, a2, a3, b1, b2 ,b3;
    double t1;
    CStep2(CStep1 cstep1, CCoord coord){  //определение расстояния точки t1 от точки стояния пеленгатора P1                                                  
        a1 = cstep1.cos_ah * cstep1.cos_bx - cstep1.cos_ax * cstep1.cos_bh;
        a2 = cstep1.cos_ay * cstep1.cos_bh - cstep1.cos_ah * cstep1.cos_by;
        a3 = cstep1.cos_ax * cstep1.cos_by - cstep1.cos_ay * cstep1.cos_bx;
        b1 = cstep1.cos_ax * (coord.h2 - coord.h) - cstep1.cos_ah * (coord.x2 - coord.x);
        b2 = cstep1.cos_ah * (coord.y2 - coord.y) - cstep1.cos_ay * (coord.h2 - coord.h);
        b3 = cstep1.cos_ay * (coord.x2 - coord.x) - cstep1.cos_ax * (coord.y2 - coord.y);
        t1 = -((a1*b1 + a2*b2 + a3*b3)/(pow(a1,2) + pow(a2,2) + pow(a3,3)));
    }

};

class CStep3{
    public:
    double a1, a2, a3, b1, b2, b3;
    double t2;
    CStep3 (CStep1 cstep1, CCoord coord){  //определение расстояния точки t2 от точки стояния пеленгатора P2    
            a1 = cstep1.cos_bh * cstep1.cos_ax - cstep1.cos_bx * cstep1.cos_ah;
            a2 = cstep1.cos_by * cstep1.cos_ah - cstep1.cos_bh * cstep1.cos_ay;
            a3 = cstep1.cos_bx * cstep1.cos_ay - cstep1.cos_by * cstep1.cos_ax;
            b1 = cstep1.cos_bx * (coord.h2 - coord.h) - cstep1.cos_bh * (coord.x2 - coord.x);
            b2 = cstep1.cos_bh * (coord.y2 - coord.y) - cstep1.cos_by * (coord.h2 - coord.h);
            b3 = cstep1.cos_by * (coord.x2 - coord.x) - cstep1.cos_bx * (coord.y2 - coord.y);
            t2 = -((a1 * b1 + a2 * b2 + a3 * b3)/(pow(a1,2) + pow(a2,2) + pow(a3,3)));      
        }
};

class CStep4 { 
    public:
    double Xt1, Yt1, Ht1, Xt2, Yt2, Ht2;
    public:
    CStep4(CStep2 cstep2, CCoord coord, CStep1 cstep1, CStep3 cstep3){ // определяются координаты точки Pt1 и точки Рt 2
        Xt1 = coord.x + cstep2.t1 * cstep1.cos_bx; //определяются координаты Pt1
        Yt1 = coord.y + cstep2.t1 * cstep1.cos_by;
        Ht1 = coord.h + cstep2.t1 * cstep1.cos_bh; 
        Xt2 = coord.x2 + cstep3.t2 * cstep1.cos_bx; //определяются координаты Pt2
        Yt2 = coord.y2 + cstep3.t2 * cstep1.cos_by;
        Ht2 = coord.h2 + cstep3.t2 * cstep1.cos_bh;   
    }
    
};

class CStep5 {
public:
    int Cp;
    double d, dr;
public:
    CStep5 ( CStep2 cstep2, CStep3 cstep3, CStep4 cstep4){ // вычисляется значение признака Ср совместимости пеленгов p1 и р2
        d = sqrt(pow(cstep4.Xt1 - cstep4.Xt2,2) + pow(cstep4.Yt1 - cstep4.Yt2,2) + pow(cstep4.Ht1 - cstep4.Ht2,2)); //расстояние между точками Рt1 и Pt2
        dr = d * cstep2.t1 + d + cstep3.t2; //максимальное возможное расстояние между пеленгами p1 и р2

        if(cstep2.t1 >= 0 && cstep3.t2 >= 0){
            if(d < dr){
                Cp = 1;
            }
        }
        else{
            Cp = 0;
            cout << "Decoy target." << endl;
            exit (0);
        }
    }
    
    
};

class CStep6{           //определяются выходные данные - координаты точки Ps на отрезке Рt1 Рt2
    public:
    double x, y, h;
    public:
    CStep6(CStep2 cstep2, CStep3 cstep3, CStep4 cstep4){
        x = (cstep4.Xt1 * cstep2.t1 + cstep4.Xt2 * cstep3.t2) / (cstep2.t1 + cstep3.t2);
        y = (cstep4.Yt1 * cstep2.t1 + cstep4.Yt2 * cstep3.t2) / (cstep2.t1 + cstep3.t2);
        h = (cstep4.Ht1 * cstep2.t1 + cstep4.Ht2 * cstep3.t2) / (cstep2.t1 + cstep3.t2); 
        
    }

};

class CTriangulate { // Класс создан для вычисления всех шагов триангуляции
    public:          // Последовательное создание объектов классов каждого шага
                     // И передача данных от одного объекта в другой
    double x, y, h;
    
    CTriangulate(CCoord coord, CPelengs cpeleng){   
    CStep1 cstep1 = CStep1(cpeleng);

    CStep2 cstep2 = CStep2(cstep1, coord);
   
    CStep3 cstep3 = CStep3(cstep1, coord);
    
    CStep4 cstep4 = CStep4(cstep2, coord, cstep1, cstep3);

    CStep5 cstep5 = CStep5(cstep2, cstep3, cstep4);

    CStep6 cstep6 = CStep6(cstep2, cstep3, cstep4);
    
    x = cstep6.x;
    y = cstep6.y;
    h = cstep6.h;
    }
};
//по результатам проведенных вычислений определяют координаты цели
//и ставят цель на сопровождение 

int main(){
    //Input
    CCoord coord(100.5, 250.5, 152.5, 157.5, 132.5, 152.5 );  // Ввод пеленгаторов
    coord.f_Print();
    
    CPelengs cpeleng(12.5, 111.5, 232.3, 323.5); // Ввод пеленгов 
    cpeleng.f_Print();

    
    // Computing 
    CTriangulate triangulation = CTriangulate(coord, cpeleng);    
    

    int x = triangulation.x;
    int y = triangulation.y;
    int h = triangulation.h;
    
    cout << "x: " << x << endl;
    cout << "y: " << y << endl;
    cout << "h: " << h << endl; 

    return 0;
}
