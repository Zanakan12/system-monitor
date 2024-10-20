<<<<<<< HEAD
# Moniteur de Système

## Objectifs et Exigences du Projet
Ce projet vise à surveiller les ressources du système, notamment :
- CPU
- RAM
- SWAP
- Ventilateur
- Réseau

Il utilise **Dear ImGui** pour l'interface graphique et le système de fichiers **/proc** pour obtenir des informations sur le système. Le projet inclut des fonctionnalités spécifiques telles que des sections avec onglets, des graphiques et des affichages visuels.

## Guide Étape par Étape

### 1. Configurer l'Environnement de Projet
- Cloner le dépôt à partir de l'URL fournie : 
  ```
  git clone https://zone01normandie.org/git/draftandj/system-monitor
  ```
- Installer les dépendances nécessaires :
  ```bash
  sudo apt install libsdl2-dev
  ```
- S'assurer d'avoir le compilateur C++ et les outils de construction nécessaires installés.

### 2. Comprendre la Structure du Système de Fichiers
Familiarisez-vous avec la structure du système de fichiers fournie :
```
text
├── header.h
├── imgui
│   └── lib
│       ├── backend
│       ├── gl3w
│       └── ...
├── main.cpp
├── Makefile
├── mem.cpp
├── network.cpp
└── system.cpp
```

### 3. Intégrer Dear ImGui
- Ajouter les fichiers Dear ImGui au projet.
- Utiliser les fichiers de backend fournis (`imgui_impl_sdl.cpp`, `imgui_impl_sdl.h`, etc.) pour gérer les entrées et le rendu.
- Comprendre la différence entre les interfaces graphiques en mode immédiat et en mode retenu.

### 4. Récupérer les Informations du Système
Utiliser le système de fichiers **/proc** pour recueillir des informations sur le système :
- CPU : `/proc/cpuinfo`
- Mémoire : `/proc/meminfo`
- Processus : `/proc/[pid]`
- Réseau : `/proc/net/dev`
- Autres informations : `/proc/sys`, `/proc/uptime`, etc.

### 5. Implémenter les Fonctionnalités de Surveillance du Système
- **Informations sur le Système** : Afficher le type de système d'exploitation, l'utilisateur connecté, le nom de l'ordinateur (hostname) et le nombre total de tâches/processus.
- **Surveillance de la CPU, du Ventilateur et de la Température** : Créer une section avec onglets pour ces informations, utiliser des graphiques pour afficher les métriques de performance.
- **Surveillance de la Mémoire et des Processus** : Afficher l'utilisation de la mémoire physique (RAM) et de la mémoire virtuelle (SWAP) avec des affichages visuels.
- **Surveillance du Réseau** : Afficher les informations de réseau IPv4 et visualiser l'utilisation du réseau.

### 6. Implémenter l'Interface Graphique avec Dear ImGui
- Utiliser `main.cpp` comme point d'entrée principal pour rendre l'interface graphique.
- Créer des onglets, des fenêtres et des widgets en utilisant les fonctions Dear ImGui.

### 7. Construire et Tester l'Application
- Utiliser le Makefile fourni pour construire l'application.
- Tester l'application pour s'assurer que toutes les fonctionnalités fonctionnent correctement.

## Architecture

### Composants
- **Collecteur d'Informations sur le Système** : Responsable de la lecture des données à partir de **/proc**.
- **Render de l'Interface Graphique** : Utilise Dear ImGui pour rendre l'interface.
- **Modèle de Données** : Contient l'état actuel des ressources du système.
- **Logique de l'Interface Graphique** : Gère les interactions de l'utilisateur.

### Flux
- **Initialisation** : Configurer le backend Dear ImGui et initialiser le modèle de données.
- **Boucle Principale** : Recueillir les informations sur le système, mettre à jour le modèle de données et rendre l'interface graphique.
- **Gestion des Événements** : Répondre aux entrées de l'utilisateur et mettre à jour l'interface graphique.

---

En suivant ces étapes et en comprenant l'architecture, vous pouvez créer efficacement un moniteur de système fonctionnel.
=======
# Moniteur de Système

## Objectifs et Exigences du Projet
Ce projet vise à surveiller les ressources du système, notamment :
- CPU
- RAM
- SWAP
- Ventilateur
- Réseau


Il utilise **Dear ImGui** pour l'interface graphique et le système de fichiers **/proc** pour obtenir des informations sur le système. Le projet inclut des fonctionnalités spécifiques telles que des sections avec onglets, des graphiques et des affichages visuels.
![system](https://github.com/user-attachments/assets/038d01b7-b5f8-4f4e-8adf-19083155a82b)
![mem](https://github.com/user-attachments/assets/defbc637-f1d6-471c-a012-7a7ed59a6d94)

## Guide Étape par Étape

### 1. Configurer l'Environnement de Projet
- Cloner le dépôt à partir de l'URL fournie : 
  ```
  git clone https://zone01normandie.org/git/draftandj/system-monitor
  ```
- Installer les dépendances nécessaires :
  ```bash
  sudo apt install libsdl2-dev
  ```
- S'assurer d'avoir le compilateur C++ et les outils de construction nécessaires installés.

### 2. Comprendre la Structure du Système de Fichiers
Familiarisez-vous avec la structure du système de fichiers fournie :
```
text
├── header.h
├── imgui
│   └── lib
│       ├── backend
│       ├── gl3w
│       └── ...
├── main.cpp
├── Makefile
├── mem.cpp
├── network.cpp
└── system.cpp
```

### 3. Intégrer Dear ImGui
- Ajouter les fichiers Dear ImGui au projet.
- Utiliser les fichiers de backend fournis (`imgui_impl_sdl.cpp`, `imgui_impl_sdl.h`, etc.) pour gérer les entrées et le rendu.
- Comprendre la différence entre les interfaces graphiques en mode immédiat et en mode retenu.

### 4. Récupérer les Informations du Système
Utiliser le système de fichiers **/proc** pour recueillir des informations sur le système :
- CPU : `/proc/cpuinfo`
- Mémoire : `/proc/meminfo`
- Processus : `/proc/[pid]`
- Réseau : `/proc/net/dev`
- Autres informations : `/proc/sys`, `/proc/uptime`, etc.

### 5. Implémenter les Fonctionnalités de Surveillance du Système
- **Informations sur le Système** : Afficher le type de système d'exploitation, l'utilisateur connecté, le nom de l'ordinateur (hostname) et le nombre total de tâches/processus.
- **Surveillance de la CPU, du Ventilateur et de la Température** : Créer une section avec onglets pour ces informations, utiliser des graphiques pour afficher les métriques de performance.
- **Surveillance de la Mémoire et des Processus** : Afficher l'utilisation de la mémoire physique (RAM) et de la mémoire virtuelle (SWAP) avec des affichages visuels.
- **Surveillance du Réseau** : Afficher les informations de réseau IPv4 et visualiser l'utilisation du réseau.

### 6. Implémenter l'Interface Graphique avec Dear ImGui
- Utiliser `main.cpp` comme point d'entrée principal pour rendre l'interface graphique.
- Créer des onglets, des fenêtres et des widgets en utilisant les fonctions Dear ImGui.

### 7. Construire et Tester l'Application
- Utiliser le Makefile fourni pour construire l'application.
- Tester l'application pour s'assurer que toutes les fonctionnalités fonctionnent correctement.

## Architecture

### Composants
- **Collecteur d'Informations sur le Système** : Responsable de la lecture des données à partir de **/proc**.
- **Render de l'Interface Graphique** : Utilise Dear ImGui pour rendre l'interface.
- **Modèle de Données** : Contient l'état actuel des ressources du système.
- **Logique de l'Interface Graphique** : Gère les interactions de l'utilisateur.

### Flux
- **Initialisation** : Configurer le backend Dear ImGui et initialiser le modèle de données.
- **Boucle Principale** : Recueillir les informations sur le système, mettre à jour le modèle de données et rendre l'interface graphique.
- **Gestion des Événements** : Répondre aux entrées de l'utilisateur et mettre à jour l'interface graphique.

---

En suivant ces étapes et en comprenant l'architecture, vous pouvez créer efficacement un moniteur de système fonctionnel.
>>>>>>> ab57b2c0b980103ac98edf2838f5b3192fe25f0c
