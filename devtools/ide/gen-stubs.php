<?php

/**
 * This scripts generates the stubs to be used on IDEs
 *
 * Change the DAO_DIR constant to point to the dev/ directory in the Dao source code
 *
 * php ide/gen-stubs.php
 */

if (!extension_loaded('dao')) {
	throw new Exception("Dao extension isn't installed");
}

define('DAO_DIR' , __DIR__.'/../../ext/');

if (!file_exists(DAO_DIR)) {
	throw new Exception("CPHALCON directory does not exist");
}

class Stubs_Generator
{

	protected $_docs = array();

    protected $_classDocs = array();

	public function __construct($directory)
	{
		$this->_scanSources($directory);
	}

    protected function _scanSources($directory)
    {
        /** @var RecursiveDirectoryIterator[] $iterator */
        $iterator = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($directory , FilesystemIterator::SKIP_DOTS));
        foreach ( $iterator as $item ) {
            if ( $item->getExtension() == 'c' ) {
                if ( strpos($item->getPathname() , 'kernel') === false ) {
                    $this->_getDocs($item->getPathname());
                }
            }
        }
    }

	protected function _getDocs($file)
	{
		$firstDoc = true;
		$openComment = false;
		$nextLineMethod = false;
		$comment = '';
		foreach (file($file) as $line) {
			if (trim($line) == '/**') {
				$openComment = true;
			}
			if ($openComment === true){
				$comment .= $line;
			} else {
				if ($nextLineMethod === true) {
					if (preg_match('/^PHP_METHOD\(([a-zA-Z0-9\_]+), (.*)\)/', $line, $matches)) {
						$this->_docs[$matches[1]][$matches[2]] = trim($comment);
						$className = $matches[1];
					} else {
						if (preg_match('/^PHALCON_DOC_METHOD\(([a-zA-Z0-9\_]+), (.*)\)/', $line, $matches)) {
							$this->_docs[$matches[1]][$matches[2]] = trim($comment);
							$className = $matches[1];
						} else {
							if ($firstDoc===true) {
								$classDoc = $comment;
								$firstDoc = false;
								$comment = '';
							}
						}
					}
					$nextLineMethod = false;
				} else {
					$comment = '';
				}
			}
			if ($openComment === true) {
				if (trim($line) == '*/') {
					$openComment = false;
					$nextLineMethod = true;
				}
			}
			if (preg_match('/^PHALCON_INIT_CLASS\(([a-zA-Z0-9\_]+)\)/', $line, $matches)) {
				$className = $matches[1];
			}
		}
		if (isset($classDoc)) {
			if (!isset($className)) {
				return null;
			}
			if (!isset($this->_classDocs[$className])) {
				$this->_classDocs[$className] = $classDoc;
			}
		}
	}

	protected function _findClassName($classDoc)
	{


		foreach (explode("\n", $classDoc) as $line) {
			echo $line;
		}
		return null;
	}

	public function getDocs()
	{
		return $this->_docs;
	}

	public function getClassDocs()
	{
		return $this->_classDocs;
	}

}

$version = Dao\Version::get();
$versionPieces = explode(' ', $version);
$genVersion = $versionPieces[0];

$api = new Stubs_Generator(DAO_DIR);

$classDocs = $api->getClassDocs();
$docs = $api->getDocs();

$allClasses = array_merge(get_declared_classes(), get_declared_interfaces());

foreach ($allClasses as $className) {

	if (!preg_match('#^Dao#', $className)) {
		continue;
	}

	$pieces = explode("\\", $className);
	$namespaceName = join("\\", array_slice($pieces, 0, count($pieces)-1));
	$normalClassName = join('', array_slice($pieces, -1));

	$source ='<?php '.PHP_EOL.PHP_EOL;
	$source.='namespace '.$namespaceName.' {'.PHP_EOL.PHP_EOL;

	$simpleClassName = str_replace("\\", "_", $className);
	if (isset($classDocs[$simpleClassName])) {
		foreach (explode("\n", $classDocs[$simpleClassName]) as $commentPiece) {
			$source .= "\t" . $commentPiece . "\n";
		}
	}

	$reflector = new ReflectionClass($className);

	$typeClass = '';
	if (!$reflector->isInterface()) {
		if ($reflector->isAbstract() == true) {
			$typeClass = 'abstract ';
		}
		if ($reflector->isFinal() == true) {
			$typeClass = 'final ';
		}
	}

	$extends = $reflector->getParentClass();
	if ($reflector->isInterface()) {
		if ($extends) {
			$source .= "\t" . 'interface ' . $normalClassName . ' extends \\'.$extends->name.' {'.PHP_EOL;
		} else {
			$source .= "\t" . 'interface ' . $normalClassName . ' {' . PHP_EOL;
		}
	} else {

		$implements = $reflector->getInterfaceNames();
		if ($extends) {
			$source.="\t".$typeClass.'class '.$normalClassName.' extends \\'.$extends->name;
		} else {
			$source.="\t".$typeClass.'class '.$normalClassName;
		}
		if ($implements) {
			$source .= " implements \\" . implode(', \\', $implements);
		}

		$source .= ' {' . PHP_EOL;
	}

	if (isset($docs[$simpleClassName])) {
		$docMethods = $docs[$simpleClassName];
	} else {
		$docMethods = array();
	}

	// constants
	foreach ($reflector->getConstants() as $constant => $value) {
		$source.= PHP_EOL . "\t\t" . 'const '.$constant.' = '.$value.';'.PHP_EOL;
	}

	// public and protected properties
	foreach ($reflector->getProperties() as $property) {
		if ($property->getDeclaringClass()->name == $className) {
			$source.= PHP_EOL . "\t\t" . implode(' ', Reflection::getModifierNames($property->getModifiers())).' $'.$property->name.';'.PHP_EOL;
		}
	}

	if ($className == 'Dao\Di\Injectable') {
		$source .= '
		/**
 		 * @var \Dao\Mvc\Dispatcher|\Dao\Mvc\DispatcherInterface
 		 */
		public $dispatcher;

		/**
 		 * @var \Dao\Mvc\Router|\Dao\Mvc\RouterInterface
 		 */
		public $router;

		/**
 		 * @var \Dao\Mvc\Url|\Dao\Mvc\UrlInterface
 		 */
		public $url;

		/**
 		 * @var \Dao\Http\Request|\Dao\HTTP\RequestInterface
 		 */
		public $request;

		/**
 		 * @var \Dao\Http\Response|\Dao\HTTP\ResponseInterface
 		 */
		public $response;

		/**
 		 * @var \Dao\Http\Response\Cookies|\Dao\Http\Response\CookiesInterface
 		 */
		public $cookies;

		/**
 		 * @var \Dao\Filter|\Dao\FilterInterface
 		 */
		public $filter;

		/**
 		 * @var \Dao\Flash\Direct
 		 */
		public $flash;

		/**
 		 * @var \Dao\Flash\Session
 		 */
		public $flashSession;

		/**
 		 * @var \Dao\Session\Adapter\Files|\Dao\Session\Adapter|\Dao\Session\AdapterInterface
 		 */
		public $session;

		/**
 		 * @var \Dao\Events\Manager
 		 */
		public $eventsManager;

		/**
 		 * @var \Dao\Db
 		 */
		public $db;

		/**
 		 * @var \Dao\Security
 		 */
		public $security;

		/**
 		 * @var \Dao\Crypt
 		 */
		public $crypt;

		/**
 		 * @var \Dao\Tag
 		 */
		public $tag;

		/**
 		 * @var \Dao\Escaper|\Dao\EscaperInterface
 		 */
		public $escaper;

		/**
 		 * @var \Dao\Annotations\Adapter\Memory|\Dao\Annotations\Adapter
 		 */
		public $annotations;

		/**
 		 * @var \Dao\Mvc\Model\Manager|\Dao\Mvc\Model\ManagerInterface
 		 */
		public $modelsManager;

		/**
 		 * @var \Dao\Mvc\Model\MetaData\Memory|\Dao\Mvc\Model\MetadataInterface
 		 */
		public $modelsMetadata;

		/**
 		 * @var \Dao\Mvc\Model\Transaction\Manager
 		 */
		public $transactionManager;

		/**
 		 * @var \Dao\Assets\Manager
 		 */
		public $assets;

		/**
		 * @var \Dao\Di|\Dao\DiInterface
	 	 */
		public $di;

		/**
		 * @var \Dao\Session\Bag
	 	 */
		public $persistent;

		/**
 		 * @var \Dao\Mvc\View|\Dao\Mvc\ViewInterface
 		 */
		public $view;
		';
	}

	// methods
	foreach ($reflector->getMethods() as $method) {
		if ($method->getDeclaringClass()->name == $reflector->name) {

			$source.=PHP_EOL;
			if (isset($docMethods[$method->name])) {
				$docMethods[$method->name] = str_replace(' Dao', ' \Dao', $docMethods[$method->name]);
				foreach (explode("\n", $docMethods[$method->name]) as $commentPiece) {
					$source.="\t\t".$commentPiece."\n";
				}
			}

			$modifiers = Reflection::getModifierNames($method->getModifiers());
			if ($reflector->isInterface()) {
				$modifiers = array_intersect($modifiers, array('static', 'public'));
			}
			$source.= "\t\t".implode(' ', $modifiers).' function '.$method->name.'(';

			$parameters = array();
			foreach ($method->getParameters() as $parameter) {
			    $strParameter = '';
                $hintedClass = $parameter->getType();
                if (is_object($hintedClass)) {
                    $strParameter = '\\' . $hintedClass->getName() . ' ';
                }
				if ($parameter->isOptional()) {
					if($parameter->isDefaultValueAvailable()){
						$strParameter .= '$' . $parameter->name . '=' . $parameter->getDefaultValue();
					} else {
						$strParameter .= '$' . $parameter->name . '=null';
					}
				} else {
					$strParameter .= '$' . $parameter->name;
				}
				$parameters[] = $strParameter;
			}
			if ($reflector->isInterface() || $method->isAbstract()) {
				$source.=join(', ', $parameters).');'.PHP_EOL.PHP_EOL;
			} else {
				$source.=join(', ', $parameters).'){ }'.PHP_EOL.PHP_EOL;
			}
		}
	}

	$source.="\t".'}'.PHP_EOL;

	$source.='}'.PHP_EOL;

	$path = __DIR__ . '/' . $genVersion . '/' . str_replace("\\", DIRECTORY_SEPARATOR, $namespaceName);
	if (!is_dir($path)) {
		mkdir($path, 0777, true);
	}

	file_put_contents($path . DIRECTORY_SEPARATOR . $normalClassName . '.php', $source);
}
