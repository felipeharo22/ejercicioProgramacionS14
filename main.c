#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

int main() {
    int opcion;

    do {
        printf("\n======================================================\n");
        printf(" SISTEMA SGIC - CONCESIONARIA RUEDAS DE ORO\n");
        printf("======================================================\n");
        printf("1. Agregar nuevo vehiculo al inventario\n");
        printf("2. Consultar todos los vehiculos disponibles\n");
        printf("3. Busqueda multicriterio por preferencias\n");
        printf("4. Registrar nuevo cliente\n");
        printf("5. Mostrar todos los clientes\n");
        printf("6. Registrar nuevo vendedor\n");
        printf("7. Mostrar vendedores (solo acumulado)\n");
        printf("8. Mostrar historial general de ventas\n");
        printf("9. Registrar una venta\n");
        printf("10. Salir del sistema\n");
        printf("======================================================\n");
        
        opcion = validarEnteroConRango(1, 10, "Seleccione una opcion");

        switch (opcion) {
            case 1: agregarVehiculo(); break;
            case 2: consultarDisponibilidad(); break;
            case 3: buscarVehiculos(); break;
            case 4: registrarCliente(); break;
            case 5: mostrarClientes(); break;
            case 6: registrarVendedor(); break;
            case 7: mostrarVendedores(); break;
            case 8: mostrarHistorialVentas(); break; // Nueva opcion agregada
            case 9: registrarVenta(); break;
            case 10: printf("\nSaliendo y guardando los cambios de forma segura...\n"); break;
        }
    } while (opcion != 10);

    return 0;
}
