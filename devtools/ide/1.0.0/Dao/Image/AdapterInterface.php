<?php 

namespace Dao\Image {

	interface AdapterInterface {

		public function resize($width=null, $height=null, $master=null);


		public function liquidRescale($width, $height, $delta_x=null, $rigidity=null);


		public function crop($width, $height, $offset_x=null, $offset_y=null);


		public function rotate($degrees);


		public function flip($direction);


		public function sharpen($amount);


		public function reflection($height=null, $opacity=null, $fade_in=null);


		public function watermark($watermark, $offset_x=null, $offset_y=null, $opacity=null);


		public function text($text, $offset_x=null, $offset_y=null, $opacity=null, $color=null, $size=null, $fontfile=null);


		public function line($sx, $sy, $ex, $ey, $color=null);


		public function polygon($coordinates, $color=null);


		public function mask($mask);


		public function background($color, $opacity=null);


		public function blur($radius=null);


		public function pixelate($amount=null);


		public function save($file=null, $opacity=null, $interlacing=null);


		public function render($type=null, $opacity=null, $interlacing=null);

	}
}
