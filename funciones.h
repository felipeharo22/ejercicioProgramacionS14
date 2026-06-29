#ifndef FUNCIONES_H
#define FUNCIONES_H

#define MAX_STR 50
#define MAX_CEDULA 15

typedef struct {
    int id;
    char marca[MAX_STR];
    char modelo[MAX_STR];
    char tipo[MAX_STR];
    char estado[MAX_STR];
    float precio;
    int disponible; 
} Vehiculo;

typedef struct {
    char cedula[MAX_CEDULA];
    char nombre[MAX_STR];
    char telefono[MAX_CEDULA];
} Cliente;

typedef struct {
    int id;
    char nombre[MAX_STR];
    float comisionTotal; 
} Vendedor;

typedef struct {
    char cedulaCliente[MAX_CEDULA];
    int idVendedor;
    int idVehiculo;
} Venta;

// Prototipos de funciones principales
void agregarVehiculo();
void consultarDisponibilidad();
void buscarVehiculos();
void registrarCliente();
void mostrarClientes();      
void registrarVendedor();    
void mostrarVendedores();    
void mostrarHistorialVentas(); // Nueva funcion
void registrarVenta();

// Prototipos de funciones de validacion
void limpiarBuffer();
void leerCadena(char *mensaje, char *cadena, int longitud);
int validarEnteroConRango(int min, int max, char *mensaje);
float validarFloat(float min, float max, char *mensaje);

#endif
