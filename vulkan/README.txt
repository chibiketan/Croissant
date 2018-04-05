1. Installer Python 3.*
2. Changer le code page de la console pour l'unicode : chcp 65001
3. Lancer l'installation de conan à l'aide de pip (lancé 2 fois pour compléter l'installation sans erreurs) : pip install conan 
4. Ajouter le repos bintray dans conan : conan remote add bintray-repos https://api.bintray.com/conan/bincrafters/public-conan 
5. Créer un dossier de build et se placer dedans.
6. Installer les dépendances avec conan : conan install .. --build missing
7. TODO : ajout le dossier d'include du SDK vulkan