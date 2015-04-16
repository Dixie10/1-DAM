#include <iostream>
#include <stdlib.h>

using namespace std;

//Definición de las Clases
class Polinomio{
    //Atributos de la Clase
private:
    double* coef;
    int grado; //el grado actual del polinomio (las UTILES)
    int max_grado; //la memoria reservada y disponible para meter coeficientes (la DIMENSIÓN).
    
    void inicializar();
    void cambiarmemoria(int n);
    //grado <= max_grado SE DEBE CUMPLIR ESTA RESTRICCIÓN!!!!!!!! SI NO VIOLAMOS SEGMENTO DE COEF!!!!
    
    //Métodos de la Clase
public:
    //MÉTODOS BÁSICOS
    Polinomio(); //constructor por defecto
    Polinomio(int max_grado); //constructor por parámetros
    //Polinomio(const Polinomio &p); //constructo por copia
    ~Polinomio(); //destructor
    void setCoeficiente(int i, double coeficiente);
    inline void setGrado(int grado) { this->grado = grado;}
    inline void setMaxGrado(int grado) { this->max_grado = grado;}
    inline double getCoeficiente (int i) {return coef[i];}
    inline int getGrado() {return grado;}
    inline int getMaxGrado() {return max_grado;}
    void print() const; //imprime
    
    //MÉTODOS AVANZADO
    Polinomio sumar_v1 (Polinomio &p); //res = p1.sumar(p2)
    void sumar_v2 (const Polinomio &p1, const Polinomio &p2); //res.sumar(p1,p2)
    void sumar_v3 (const Polinomio &p); //p1.sumar(p2) en p1 se almacena tb el resultado
};

//Programación de los Métodos de las Clases
/**
 * @brief Constructor de la Clase Polinomio. Por defecto reserva memoria para 10 coeficientes y lo inicializa a 0.
 */
Polinomio::Polinomio(){
    grado = 0;
    max_grado = 10;
    coef = new double[max_grado+1];
    if (coef == 0){
        cerr << "Error en la reserva de memoria del Polinomio. Se abortará la ejecución!" << endl;
        exit(-1);
    }
    //inicializamos
    for (int i=0; i <= max_grado; i++)
        coef[i] = 0.0;
}

/**
 * @brief Destructor de la Clase Polinomio. Por defecto reserva memoria para 10 coeficientes y lo inicializa a 0.
 */
Polinomio::~Polinomio(){
    delete [] coef;
    grado = 0;
    max_grado = 0;
}

/**
 * @brief Asigna el coeficiente a la posición i del polinomio
 * @param i es el grado del monomio en el que irá el coeficiente
 * @param coeficiente es el valor numérico que tendrá el monomio
 * @version 1.1
 */
void Polinomio::setCoeficiente(int i, double coeficiente){
    if (i >= 0){
        if (i > max_grado){ //no tengo espacio reservado
            //RESERVAR NUEVA MEMORIA
            double *aux = new double[i*2]; //el doble de i
            if (!aux){
                cerr << "Error en la reserva de memoria del Polinomio. Se abortará la ejecución!" << endl;
                exit(-1);
            }
            //COPIAR LO QUE HABÍA
            for (int k = 0; k <= max_grado; k++)
                aux[k] = coef[k];
            //ASIGNAR EL NUEVO COEFICIENTE Y CAMBIAR EL GRADO Y MAX_GRADO
            aux[i] = coeficiente;
            grado = i;
            max_grado = (2*i)-1;
            //ELIMINAR LA MEMORIA DINÁMICA DE LOS ANTIGUOS COEFICIENTES
            delete []coef;
            //CAMBIAR LOS PUNTEROS
            coef = aux;
        }else{ //i < max_grado
            if(i <= grado){
                coef[i] = coeficiente;
            }
            else{ //i > grado
                coef[i] = coeficiente;
                grado = i;
            }
        }
    }
    else{
        cerr << "No se puede asignar un coeficiente a un grado negativo!" << endl;
    }
}

/**
 * @brief Imprime el polinomio.
 */
void Polinomio::print() const{
    cout << "p(x) = ";
    for(int i = max_grado; i > 0; i--)
        cout << "(" << coef[i] << ")x^" << i << " + ";
    //imprimo el último coeficiente sin el signo + del final y sin la x
    cout << coef[0] << endl;
}

Polinomio Polinomio::sumar_v1 (Polinomio &p){
    int gradomaximo = 0;
    int grado_actual = 0;
    //1º) Distinguir qué polinomio tiene más grado y ajustamos sus tamaños para igualarlos sin cambiar sus coeficientes
    if (this->getGrado() > p.getGrado()){
        gradomaximo = this->getGrado();
        //Cambiamos el tamaño del vector de coeficientes sin cambiar el grado del polinomio
        grado_actual = p.getGrado();
        p.setCoeficiente(gradomaximo,0);
        p.setGrado(grado_actual);
    }
    else{
        gradomaximo = p.getGrado();
        //Cambiamos el tamaño del vector de coeficientes sin cambiar el grado del polinomio
        grado_actual = this->getGrado();
        this->setCoeficiente(gradomaximo,0);
        this->setGrado(grado_actual);
    }
    //2º) Crear el polinomio resultado con el gradomaximo
    Polinomio resultado;
    //3º) Recorrer apropiadamente los dos polinomios e ir sumando y almacenando
    for(int i = 0; i < resultado.getMaxGrado(); i++)
        resultado.setCoeficiente(i, p.getCoeficiente(i) + this->getCoeficiente(i));
    //4º) Devuelvo el polinomio
    return resultado;
}


void Polinomio::sumar_v2 (const Polinomio &p1, const Polinomio &p2){ //res.sumar(p1,p2)
    int nuevo_grado;
    int nuevo_max_grado;
    bool es_mas_grande_p1 = false;
    //1º) Distinguir cual de los dos polinomios tiene mayor grado
    if (p1->getGrado() > p2.getGrado()){
        nuevo_grado = p1.getGrado();
        nuevo_max_grado = p1.getMaxGrado();
        es_mas_grande_p1 = true;
    }else{
        nuevo_grado = p2.getGrado();
        nuevo_max_grado = p2.getMaxGrado();
        es_mas_grande_p1 = false;
    }
    //2º) Reservo la memoria pertinente para mi objeto actual, en caso de ser necesario
    if (this->getMaxGrado() < nuevo_max_grado){ //SI NECESITO AUMENTAR MEMORIA
        //RESERVAR NUEVA MEMORIA
        double *aux = new double[nuevo_max_grado+1]; //el tamaño del polinomio con más coeficientes
        if (!aux){
            cerr << "Error en la reserva de memoria del Polinomio. Se abortará la ejecución!" << endl;
            exit(-1);
        }
        //ELIMINAR LA MEMORIA DINÁMICA DE LOS ANTIGUOS COEFICIENTES
        delete []coef;
        //CAMBIAR LOS PUNTEROS Y ACTUALIZAR MAX_GRADO
        max_grado = nuevo_max_grado; //this->setMaxGrado(nuevo_max_grado);
        coef = aux;
    }
    
    //3º) Sumo en el orden apropiado
    if (es_mas_grande_p1){
        //2.1) Copio primero el más grande, que en este caso es p1
        for (int i = 0; i < p1.getGrado(); i++)
            this->setCoeficiente(i, p1.getCoeficiente(i));
        //2.2) Le sumo ahora el más pequeño que en este caso es p2
        for (int i = 0; i < p2.getGrado(); i++)
            this->setCoeficiente(i, this->getCoeficiente(i) + p2.getCoeficiente(i));
    }else{
        //2.1) Copio primero el más grande, que en este caso es p2
        for (int i = 0; i < p2.getGrado(); i++)
            this->setCoeficiente(i, p2.getCoeficiente(i));
        //2.2) Le sumo ahora el más pequeño
        for (int i = 0; i < p1.getGrado(); i++)
            this->setCoeficiente(i, this->getCoeficiente(i) + p1.getCoeficiente(i));
    }
}


int main(){
    //Utilización de los Objetos de las Clases
    Polinomio p1; //invocación ESTÁTICA al constructor de la clase polinomio...
    p1.setCoeficiente(7, -3); //asignación normal (hay espacio espacio suficiente)
    p1.setCoeficiente(-3, 1000); //error en la asignación
    p1.setCoeficiente(11, 5); //hay que reservar memoria...
    p1.print();  //hemos cambiado la filosofía... YA NO MÁS print(p1)! USAMOS LAS FUNCIONES DEL OBJETO!!!
    
    Polinomio p2;
    p2.setCoeficiente(7, 2); //asignación normal (hay espacio espacio suficiente)
    p2.setCoeficiente(11, -3);
    p2.print();
    
    //SUMAR v1
    resultado = p1.sumar_v1(p2); //esto no funciona hasta tener el constructor por copia programado (SEGMENTATION)
    resultado.print();
    
    //SUMAR v2
    resultado.sumar_v2(p1,p2);
    
    //SUMAR v3
    p1.sumar_v3(p2);
    
    /*Polinomio *p2; //invocación DINÁMICA
     p2 = new Polinomio();
     p2->setCoeficiente(7, -3);
     p2->print();
     p2->setCoeficiente(-3, 1000); //error	
     delete p2; //aquí se invoca automáticamente de camino al destructor de la clase Polinomio*/
    
    //INVOCACIÓN 
}	



















