#include "client.h"

int main(void)
{
	/*--------------------------PARTE 2-----------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente escribi: "Hola! Soy un log"

	log_info(logger,"Hola! Soy un Log");

	/* ------- ARCHIVOS DE CONFIGURACION ------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del
	// config y los dejamos en las variables 'ip', 'puerto' y 'valor'

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config,"PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	// Loggeamos el valor, la ip y el puerto de config

	log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO: %s", puerto);
	log_info(logger, "CLAVE: %s", valor);

	/* ------------- LEER DE CONSOLA ------------- */

	leer_consola(logger);



	/*-------------------------PARTE 3------------------------------*/

	log_info(logger, "Intentando conectar...");
	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);
	log_info(logger, "Conectado al servidor!");

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);
	log_info(logger, "Envié el mensaje: %s", valor);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(void){
	t_log* nuevo_logger;

	nuevo_logger = log_create("tp0.log","Cliente",true,LOG_LEVEL_INFO);

	//Verificar si fallo
	if(nuevo_logger == NULL){
		perror("No se creo el log");
		abort();
	}

	return nuevo_logger;
}

t_config* iniciar_config(void){
	t_config* nuevo_config;

	nuevo_config = config_create("cliente.config");

	//Verificar si fallo
	if(nuevo_config == NULL){
		perror("Error al cargar el archivo de config");
		abort();
	}

	return nuevo_config;
}

void leer_consola(t_log* logger){
	char* leido;

	// La primera
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(leido != NULL && strcmp(leido, "") != 0){
		log_info(logger, "Consola: %s", leido);

		//Importante liberar la memoria
		free(leido);

		leido = readline("> ");
	}

	// ¡No te olvides de liberar la ultima!
	free(leido);
}

void paquete(int conexion){
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");

	while(leido != NULL && strcmp(leido, "") != 0){
		agregar_a_paquete(paquete, leido, strlen(leido)+1);

		//Importante liberar la memoria
		free(leido);

		leido = readline("> ");
	}
	free(leido);

	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar el paquete!
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config){
	//Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	//con las funciones de las commons y del TP mencionadas en el enunciado 
	if (logger != NULL){
		log_destroy(logger);}

	if (config != NULL){
		config_destroy(config);}

	liberar_conexion(conexion);
}
