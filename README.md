[16 tools called]

Ce projet est un **système de routage intelligent pour panneaux photovoltaïques** basé sur un ESP32-S3. Voici comment il fonctionne :

## Architecture générale

Le système surveille la puissance produite par les panneaux solaires et contrôle automatiquement un chauffage (ou autre dispositif) quand la puissance devient négative (injection sur le réseau).

## Composants principaux

### 1. **Mesure de puissance (ADC)**
- Utilise un ADC (convertisseur analogique-numérique) pour mesurer la tension, le courant et la puissance
- Calcule la puissance réelle en temps réel via le registre `P_MEAN`
- Supporte la calibration automatique avec différents modes de fonctionnement

### 2. **Contrôle du relais**
- Contrôle un GPIO (broche 18) connecté à un relais
- Active/désactive automatiquement un chauffage quand la puissance passe en dessous d'un seuil (-100W par défaut)
- Le relais est déclenché par un signal bref (impulsion)

### 3. **Serveur Web intégré**
- **Interface HTTP** : Sert des pages HTML statiques (`index.html`, `calibration.html`) depuis la partition LittleFS
- **WebSocket** : Communication temps réel sur le port 8081 (`/ws`)
- Permet de contrôler manuellement le chauffage (ON/OFF) et de visualiser les données

### 4. **Stockage**
- **LittleFS** : Système de fichiers intégré pour stocker les pages web et données de calibration
- **NVS (Non-Volatile Storage)** : Stockage persistant des paramètres de calibration

### 5. **Connexion réseau**
- Se connecte automatiquement au WiFi (SSID et mot de passe configurés via variables d'environnement)
- Fournit une interface web accessible depuis n'importe quel navigateur

## Flux de fonctionnement

1. **Démarrage** :
   - Monte la partition LittleFS
   - Se connecte au WiFi
   - Démarre le serveur HTTP/WebSocket
   - Initialise et calibre l'ADC

2. **Boucle principale** (toutes les secondes) :
   - Lit la puissance actuelle depuis l'ADC
   - Si puissance < -100W → déclenche le relais (chauffage ON)
   - Envoie les données aux clients WebSocket connectés

3. **Interface web** :
   - Boutons pour contrôle manuel du chauffage
   - Affichage temps réel des mesures (tension, courant, puissance)
   - Page de calibration pour ajuster les paramètres ADC

## Environnements de build

- **esp32-s3-devkitc-1** : Configuration standard
- **calibration_at_ib** : Mode calibration avec courant
- **calibration_no_current** : Mode calibration sans courant
- **test** : Environnement de test

Le système agit comme un **optimiseur de consommation** qui utilise l'énergie solaire excédentaire pour alimenter un chauffage plutôt que de l'injecter sur le réseau électrique.
