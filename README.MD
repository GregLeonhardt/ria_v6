#   RIA
#   Recipe Import Assist
 
#   Run these commands before first compile and run:

    sudo apt install mysql-server
    sudo mysql_secure_installation
        0           # LOW
        {password}
        Y           # Everything else
    mysql -u root -p
        ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password \
            BY 'very_strong_password';
        FLUSH PRIVILEGES;

    sudo apt install libmysqlclient-dev 
    sudo apt install libssl-dev

    mkdir ~/.LibTools
    cp {build_dir}/Translations ~/.LibTools/.

#   The goal is to be able to feed it files with various formatting models.
#   I currently have decode models for:
    BOF     Big Oven
    MMF     MealMaster
    MXP     MasterCook
    MX2     MasterCook
    NYC     Now Your'e Cooking

    * Refer to the documents directory for additional information for each
      file format.

NOTE-01:
    The application will attempt to scan "ALL" files in the directory pointed
    to by the '-id' command line parameter.  Attempts are made to exclude
    image files empty files and other stuff that won't have recipes in them.

NOTE-02:
    Files are scanned from beginning to end for recipes of all recipe formats.
    In other words recipes of all recipe formats can exist inside a single
    file.
