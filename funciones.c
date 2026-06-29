#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"

// --- FUNCIONES GENERALES DE VALIDACION ---

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void leerCadena(char *mensaje, char *cadena, int longitud) {
    printf("%s", mensaje);
    if (fgets(cadena, longitud, stdin) != NULL) {
        size_t len = strlen(cadena);
        if (len > 0 && cadena[len - 1] == '\n') {
            cadena[len - 1] = '\0';
        } else {
            limpiarBuffer();
        }
    }
}

int validarEnteroConRango(int min, int max, char *mensaje) {
    int valor, valido;
    do {
        printf("%s (entre %d y %d): ", mensaje, min, max);
        valido = scanf("%d", &valor);
        limpiarBuffer(); 
        if (valido != 1 || valor < min || valor > max) {
            printf("Error: Entrada invalida. Intente de nuevo.\n");
            valido = 0;
        }
    } while (valido == 0);
    return valor;
}

float validarFloat(float min, float max, char *mensaje) {
    float valor;
    int valido;
    do {
        printf("%s (minimo %.2f): ", mensaje, min);
        valido = scanf("%f", &valor);
        limpiarBuffer(); 
        if (valido != 1 || valor < min || valor > max) {
            printf("Error: Monto invalido. Intente de nuevo.\n");
            valido = 0;
        }
    } while (valido == 0);
    return valor;
}

// --- MODULO INVENTARIO ---

void agregarVehiculo() {
    FILE *archivo = fopen("inventario.dat", "ab");
    if (archivo == NULL) return;

    Vehiculo v;
    printf("\n--- AGREGAR NUEVO VEHICULO ---\n");
    v.id = validarEnteroConRango(1, 999999, "ID del vehiculo");
    leerCadena("Marca (ej. Chevrolet): ", v.marca, MAX_STR);
    leerCadena("Modelo (ej. LUV): ", v.modelo, MAX_STR);
    leerCadena("Tipo (ej. Camioneta, Sedan): ", v.tipo, MAX_STR);
    
    if (validarEnteroConRango(0, 1, "Estado (0 = Nuevo, 1 = Usado)") == 0) {
        strcpy(v.estado, "Nuevo");
    } else {
        strcpy(v.estado, "Usado");
    }
    
    v.precio = validarFloat(0.0f, 1000000.0f, "Precio del vehiculo $");
    v.disponible = 1;

    fwrite(&v, sizeof(Vehiculo), 1, archivo);
    fclose(archivo);
    printf("\n¡Vehiculo agregado exitosamente al inventario!\n");
}

void consultarDisponibilidad() {
    FILE *archivo = fopen("inventario.dat", "rb");
    if (archivo == NULL) {
        printf("\nEl inventario esta vacio.\n");
        return;
    }

    Vehiculo v;
    int encontrados = 0;
    
    printf("\n--- VEHICULOS DISPONIBLES EN INVENTARIO ---\n");
    printf("+---------+----------------------+----------------------+---------------+------------+--------------+\n");
    printf("| ID      | MARCA                | MODELO               | TIPO          | ESTADO     | PRECIO       |\n");
    printf("+---------+----------------------+----------------------+---------------+------------+--------------+\n");
    
    while (fread(&v, sizeof(Vehiculo), 1, archivo) == 1) {
        if (v.disponible == 1) {
            printf("| %-7d | %-20.20s | %-20.20s | %-13.13s | %-10.10s | $%11.2f |\n", 
                   v.id, v.marca, v.modelo, v.tipo, v.estado, v.precio);
            encontrados++;
        }
    }
    printf("+---------+----------------------+----------------------+---------------+------------+--------------+\n");
    if (encontrados == 0) printf("No hay vehiculos disponibles en este momento.\n");
    fclose(archivo);
}

void buscarVehiculos() {
    FILE *archivo = fopen("inventario.dat", "rb");
    if (archivo == NULL) {
        printf("\nEl inventario esta vacio.\n");
        return;
    }

    char filtroMarca[MAX_STR], filtroTipo[MAX_STR], filtroEstado[MAX_STR];
    printf("\n--- BUSQUEDA MULTICRITERIO ---\n");
    leerCadena("Ingrese la Marca deseada: ", filtroMarca, MAX_STR);
    leerCadena("Ingrese el Tipo (ej. Camioneta): ", filtroTipo, MAX_STR);
    
    if (validarEnteroConRango(0, 1, "Filtro de Estado (0 = Nuevo, 1 = Usado)") == 0) {
        strcpy(filtroEstado, "Nuevo");
    } else {
        strcpy(filtroEstado, "Usado");
    }
    
    float presupuestoMax = validarFloat(0.0f, 1000000.0f, "Presupuesto maximo $");

    Vehiculo v;
    int encontrados = 0;
    
    printf("\n--- RESULTADOS DE LA BUSQUEDA ---\n");
    printf("+---------+----------------------+----------------------+---------------+------------+--------------+\n");
    printf("| ID      | MARCA                | MODELO               | TIPO          | ESTADO     | PRECIO       |\n");
    printf("+---------+----------------------+----------------------+---------------+------------+--------------+\n");
    
    while (fread(&v, sizeof(Vehiculo), 1, archivo) == 1) {
        if (v.disponible == 1 && strcasecmp(v.marca, filtroMarca) == 0 &&
            strcasecmp(v.tipo, filtroTipo) == 0 && strcasecmp(v.estado, filtroEstado) == 0 &&
            v.precio <= presupuestoMax) {
            printf("| %-7d | %-20.20s | %-20.20s | %-13.13s | %-10.10s | $%11.2f |\n", 
                   v.id, v.marca, v.modelo, v.tipo, v.estado, v.precio);
            encontrados++;
        }
    }
    printf("+---------+----------------------+----------------------+---------------+------------+--------------+\n");
    if (encontrados == 0) printf("Ningun vehiculo cumple con todas las preferencias.\n");
    fclose(archivo);
}

// --- MODULO CLIENTES Y VENDEDORES ---

void registrarCliente() {
    FILE *archivo = fopen("clientes.dat", "ab");
    if (archivo == NULL) return;

    Cliente c;
    printf("\n--- REGISTRAR NUEVO CLIENTE ---\n");
    leerCadena("Ingrese la Cedula del cliente: ", c.cedula, MAX_CEDULA);
    leerCadena("Ingrese el Nombre completo: ", c.nombre, MAX_STR);
    leerCadena("Ingrese el Telefono: ", c.telefono, MAX_CEDULA);

    fwrite(&c, sizeof(Cliente), 1, archivo);
    fclose(archivo);
    printf("\n¡Cliente registrado exitosamente!\n");
}

void mostrarClientes() {
    FILE *archivo = fopen("clientes.dat", "rb");
    if (archivo == NULL) {
        printf("\nNo hay clientes registrados.\n");
        return;
    }

    Cliente c;
    printf("\n--- LISTADO DE CLIENTES ---\n");
    printf("+-----------------+-----------------------------------+-----------------+\n");
    printf("| CEDULA          | NOMBRE                            | TELEFONO        |\n");
    printf("+-----------------+-----------------------------------+-----------------+\n");
    while (fread(&c, sizeof(Cliente), 1, archivo) == 1) {
        printf("| %-15.15s | %-33.33s | %-15.15s |\n", c.cedula, c.nombre, c.telefono);
    }
    printf("+-----------------+-----------------------------------+-----------------+\n");
    fclose(archivo);
}

void registrarVendedor() {
    FILE *archivo = fopen("vendedores.dat", "ab");
    if (archivo == NULL) return;

    Vendedor v;
    printf("\n--- REGISTRAR NUEVO VENDEDOR ---\n");
    v.id = validarEnteroConRango(1, 9999, "ID del vendedor");
    leerCadena("Ingrese el Nombre del vendedor: ", v.nombre, MAX_STR);
    v.comisionTotal = 0.0f;

    fwrite(&v, sizeof(Vendedor), 1, archivo);
    fclose(archivo);
    printf("\n¡Vendedor registrado exitosamente!\n");
}

// Version simplificada: Solo muestra el vendedor y su acumulado
void mostrarVendedores() {
    FILE *fVend = fopen("vendedores.dat", "rb");
    if (fVend == NULL) {
        printf("\nNo hay vendedores registrados.\n");
        return;
    }

    Vendedor vend;
    printf("\n--- LISTADO DE VENDEDORES ---\n");
    printf("+---------+-----------------------------------+---------------+\n");
    printf("| ID VEND | NOMBRE VENDEDOR                   | ACUMULADO     |\n");
    printf("+---------+-----------------------------------+---------------+\n");
    
    while (fread(&vend, sizeof(Vendedor), 1, fVend) == 1) {
        printf("| %-7d | %-33.33s | $%12.2f |\n", vend.id, vend.nombre, vend.comisionTotal);
    }
    printf("+---------+-----------------------------------+---------------+\n");
    fclose(fVend);
}

// Nueva funcion: Muestra el historial de ventas plano
void mostrarHistorialVentas() {
    FILE *fVentas = fopen("ventas.dat", "rb");
    if (fVentas == NULL) {
        printf("\nNo hay ventas registradas en el historial.\n");
        return;
    }

    Venta vta;
    printf("\n--- HISTORIAL DE VENTAS ---\n");
    printf("+-----------------+-------------+--------------+\n");
    printf("| CEDULA CLIENTE  | ID VENDEDOR | ID VEHICULO  |\n");
    printf("+-----------------+-------------+--------------+\n");
    
    while (fread(&vta, sizeof(Venta), 1, fVentas) == 1) {
        printf("| %-15.15s | %-11d | %-12d |\n", vta.cedulaCliente, vta.idVendedor, vta.idVehiculo);
    }
    printf("+-----------------+-------------+--------------+\n");
    fclose(fVentas);
}

// --- MODULO TRANSACCIONAL ---

void registrarVenta() {
    printf("\n--- REGISTRO DE VENTA ---\n");
    
    char cedulaBuscada[MAX_CEDULA];
    leerCadena("Ingrese la Cedula del cliente comprador: ", cedulaBuscada, MAX_CEDULA);

    FILE *archivoCli = fopen("clientes.dat", "rb");
    if (archivoCli == NULL) {
        printf("\nBase de datos de clientes vacia. Registre al cliente primero.\n");
        return;
    }
    Cliente c;
    int clienteEncontrado = 0;
    while (fread(&c, sizeof(Cliente), 1, archivoCli) == 1) {
        if (strcmp(c.cedula, cedulaBuscada) == 0) {
            clienteEncontrado = 1;
            break;
        }
    }
    fclose(archivoCli);
    if (!clienteEncontrado) {
        printf("\nCliente no encontrado. Registrelo primero.\n");
        return;
    }

    int idVendBuscado = validarEnteroConRango(1, 9999, "Ingrese el ID del vendedor que realiza la venta");
    FILE *archivoVend = fopen("vendedores.dat", "r+b");
    if (archivoVend == NULL) {
        printf("\nBase de datos de vendedores vacia. Registre al vendedor primero.\n");
        return;
    }
    Vendedor vend;
    int vendedorEncontrado = 0;
    long posVendedor = 0; 
    
    while (fread(&vend, sizeof(Vendedor), 1, archivoVend) == 1) {
        if (vend.id == idVendBuscado) {
            vendedorEncontrado = 1;
            posVendedor = ftell(archivoVend) - sizeof(Vendedor); 
            break;
        }
    }
    if (!vendedorEncontrado) {
        printf("\nVendedor no encontrado.\n");
        fclose(archivoVend);
        return;
    }

    FILE *archivoInv = fopen("inventario.dat", "r+b");
    if (archivoInv == NULL) {
        printf("\nError: No se puede acceder al inventario.\n");
        fclose(archivoVend);
        return;
    }

    int idVehiculo = validarEnteroConRango(1, 999999, "\nIngrese el ID del vehiculo a comprar");
    Vehiculo v;
    int vehiculoEncontrado = 0;

    while (fread(&v, sizeof(Vehiculo), 1, archivoInv) == 1) {
        if (v.id == idVehiculo && v.disponible == 1) {
            vehiculoEncontrado = 1;
            
            Venta nuevaVenta;
            nuevaVenta.idVehiculo = v.id;
            nuevaVenta.idVendedor = vend.id;
            strcpy(nuevaVenta.cedulaCliente, cedulaBuscada);
            
            FILE *archivoVentas = fopen("ventas.dat", "ab");
            if (archivoVentas != NULL) {
                fwrite(&nuevaVenta, sizeof(Venta), 1, archivoVentas);
                fclose(archivoVentas);
            }

            v.disponible = 0;
            fseek(archivoInv, -sizeof(Vehiculo), SEEK_CUR); 
            fwrite(&v, sizeof(Vehiculo), 1, archivoInv);

            float comision = v.precio * 0.06f;
            vend.comisionTotal += comision;
            fseek(archivoVend, posVendedor, SEEK_SET); 
            fwrite(&vend, sizeof(Vendedor), 1, archivoVend);
            
            printf("\n¡Transaccion completada!\n");
            printf("- Vehiculo asignado a: %s\n", c.nombre);
            printf("- Comision de $%.2f asignada al vendedor: %s\n", comision, vend.nombre);
            break; 
        }
    }

    if (!vehiculoEncontrado) {
        printf("\nEl ID de vehiculo no existe o ya fue vendido.\n");
    }

    fclose(archivoInv);
    fclose(archivoVend);
}
