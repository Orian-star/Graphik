#ifndef FUNCTIONS_H
#define FUNCTIONS_H

double pars_add (char *a, char *b);
double pars_sub (char *a, char *b);
double pars_mult (char *a, char *b);
double pars_divi (char *a, char *b);
double pars_power(char *a, char *b);
double add(double a, double b);
double sub(double a, double b);
double mult(double a, double b);
double divi(double a, double b);

typedef struct Pars_Functions {
    double (*add)(char *, char *);
    double (*sub)(char *, char *);
    double (*mult)(char *, char *);
    double (*div)(char *, char *);
    double (*power)(char *, char *);
    double (*sin)(char *);
    double (*cos)(char *);
    double (*tan)(char *);
    double (*exp)(char *, char *);
    double (*log)(char *, char *);
    double (*abs)(char *);
} Pars_Functions;



void init_structs(Pars_Functions *pars_functions);



#endif