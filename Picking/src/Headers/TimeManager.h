#ifndef _TimeManager_H
#define _TimeManager_H

#include <chrono>
#include <thread>

/**
 * Se usa una clase diseñada como singleton que calcula el frame rate, solo se crea una sola vez
 * la instancia y se usa muchas veces
 */
class TimeManager {
 public:

  /**
   * Funcion que obtiene la instancia del manejador del tiempo, o en caso que sea
   * la primera vez que se invoca crea la instancia.
   * @return Instancia del manejador de tiempos.
   */
  static TimeManager& Instance() {
    static TimeManager instance;

    return instance;
  }

  /**
   * Metodo que calcula el frame rate.
   * @param writeToConsole bandera que indica si se requiere loggear a la consola el valor que se obtiene
   * @return FPS
   */
  double CalculateFrameRate(bool writeToConsole);

  /**
   * Método que obtien el tiempo actual en milisegundos
   * @return tiempo
   */
  double GetTime();

  /**
   * Método que pausa un cierto tiempo en milisegundo el hilo que se esta ejecutando
   * @param milliseconds tiempo de pausa
   */
  void Sleep(int milliseconds);

  /**
   * Atributo que almacena el tiempo total que ha pasdo desde el ultimo frame
   */
  double DeltaTime = 0;

  /**
   * Tiempo actual en milisegundos
   */
  double CurrentTime = 0;

 private:
  TimeManager() {
  }
  ;
  TimeManager(TimeManager const&);
  TimeManager& operator=(TimeManager const&);
};

#endif
