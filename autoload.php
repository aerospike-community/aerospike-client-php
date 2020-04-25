<?php
/* If you are getting this package through Composer the Aerospike\LDT autoloader
 * and Aerospike\Bytes autoloader are already registered.
 * Otherwise, you can include this file and it will handle registering the
 * autoloaders.
 */
$autoloaders = spl_autoload_functions();

if (!is_array($autoloaders) || !array_key_exists('Aerospike\\GeoJSON\\Autoloader', $autoloaders)) {
    require __DIR__. '/src/GeoJSON/Autoloader.php';
    \Aerospike\GeoJSON\Autoloader::register();
}
if (!is_array($autoloaders) || !array_key_exists('Aerospike\\Bytes', $autoloaders)) {
    spl_autoload_register(function ($class_name) {
        if ($class_name == 'Aerospike\Bytes') {
            require __DIR__. '/src/Bytes.php';
        }
    });
}
