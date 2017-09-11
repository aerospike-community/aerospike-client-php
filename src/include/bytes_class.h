#include "php.h"
/*#include "php_ini.h"
#include "ext/standard/info.h"
*/
//#include "php_aerospike.h"
//#include "types.h"


// /*
// class Bytes implements \Serializable
// {
//     /**
//      * The container for the binary-string
//      * @var string
//      */
//     public $s;
//     /**
//      * Constructor for \Aerospike\Bytes class.
//      *
//      * @param string $bin_str a PHP binary-string such as gzdeflate() produces.
//      */
//     public function __construct($bin_str) {
//         $this->s = $bin_str;
//     }
//     /**
//      * Returns a serialized representation of the binary-string.
//      * Called by serialize()
//      *
//      * @return string
//      */
//     public function serialize() {
//         return $this->s;
//     }
//     *
//      * Re-wraps the binary-string when called by unserialize().
//      *
//      * @param string $bin_str a PHP binary-string. Called by unserialize().
     
//     public function unserialize($bin_str) {
//         return $this->s = $bin_str;
//     }
//     /**
//      * Returns the binary-string held in the \Aerospike\Bytes object.
//      *
//      * @return string
//      */
//     public function __toString() {
//         return $this->s;
//     }
//     /**
//      * Unwraps an \Aerospike\Bytes object, returning the binary-string inside.
//      *
//      * @param \Aerospike\Bytes $bytes_wrap
//      * @return string
//      */
//     public static function unwrap(\Aerospike\Bytes $bytes_wrap) {
//         return $bytes_wrap->s;
//     }
// }

extern zend_class_entry *aerospike_ce;
extern zend_object_handlers aerospike_ce_handlers;

PHP_METHOD(AerospikeBytes, __construct);
PHP_METHOD(AerospikeBytes, __toString);
