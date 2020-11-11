(Dao Framework
========================

Dao is a web framework implemented as a C extension offering high performance and lower resource consumption.

Get Started
-----------

Dao is written in C with platform independence in mind. As a result, Dao is available on GNU/Linux, and Mac OS X. You can build it from sources.

### Linux/Unix/Mac

On a Unix-based platform you can easily compile and install the extension from sources.

#### Requirements

Prerequisite packages are:

* PHP 8.0.x development resources
* GCC compiler (Linux/Solaris) or Xcode (Mac)

Compilation
-----------

Follow these instructions to generate a binary extension for your platform:

```bash
git clone git://github.com/dreamsxin/dao8.git
cd dao8/ext

phpize
./configure

# or custom php path
/usr/local/php/bin/phpize
./configure --with-php-config=/usr/local/php/bin/php-config

# or debug
./configure CFLAGS="-g3 -O0 -std=gnu90 -Wall -Werror -Wno-error=uninitialized"

make -j4
sudo make install
```

Other options:
```shell
./configure --enable-storage-btree=yes --enable-storage-wiredtiger=yes \
--enable-storage-bloomfilter=yes --enable-storage-datrie=yes \
--enable-storage-lmdb=yes --enable-storage-libmdbx=yes \
--enable-storage-leveldb=yes --enable-python=yes

make -j4
sudo make install
```

Add the extension to your php.ini:

```ini
extension=dao.so
```

Test:

```shell
php --ri dao

cd dao
composer install
vendor/bin/phpunit unit-tests
```

Finally, restart the webserver.

License
-------
Dao is open source software licensed under the New BSD License. See the docs/LICENSE.txt file for more information.
