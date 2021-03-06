<?php
/**
 * This class allows creation, manipulation and transformation
 * of Images.
 * It extends a basic File class, to which it adds image editing functions.
 *
 */
define('IMAGE_RENDER_FULL', 'image_full');

/**
 * Class Image
 */
class Image extends FileObject {
  /** @var int $width */
  public $width = '';
  /** @var int $height */
  public $height = '';
  /** @var array $css */
  public $css = array();
  /** @var array $class */
  public $class = array();
  /** @var string $linkto */
  public $linkto = NULL;
  /** @var string $title */
  public $title = NULL;

  /**
   * Load image attributes.
   * @param $attributes
   */
  public function loadAttributes($attributes) {
    foreach($attributes as $attname => $attribute) {

      // Check the image size as input by the user.
      if (preg_match_all('/[0-9|auto]x[0-9|auto]/', $attname, $matches)) {
        $size = explode('x', $attname);
        $this->width = $size[0];
        $this->height = $size[1];
      }

      else switch(strtolower($attname)) {
        case 'class':
          $this->class = explode(',', $attribute);
          break;
        case 'float':
          $this->css[] = 'float:' . $attribute;
          break;
				case 'link':
          $this->linkto = $attribute;
					break;
        case 'title':
					$this->setTitle($attribute);
					break;
        case 'w':
          $this->width = $attribute;
          break;
        case 'h':
          $this->height = $attribute;
          break;

				default:
          break;
      }
    }
    if (empty($this->getTitle())) {
      $this->setTitle($this->getFileName());
    }

		// If width or height are not specified, get it from img directly. (Slow).
		if (empty($this->width) || empty($this->height)) {
		  $get_size = getimagesize($this->getRealPath());
			$this->width = $get_size[0];
			$this->height = $get_size[1];	
		}

  }

  /**
   * Set image's title.
   * @param $title
   */
  public function setTitle($title) {
    $this->title = $title;
  }

  /**
   * Return Image's title.
   * @return string
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   * Render the image.
   * @param string $mode
   * @return string
   */
  public function render($mode = IMAGE_RENDER_FULL) {
    $imgurl = (strpos($this->path, '/') !== FALSE) ? $this->path : '/' . $this->node->getName() . '/' . $this->path;
		$style = (count($this->css) > 0) ? 'style="' . implode(';', $this->css) . '" ' : '';
    $class = (count($this->class) > 0) ?  implode(' ', $this->class) : '';
    $width = ($this->width > 0) ? 'width="' . $this->width . '"' : '';
    $height = ($this->height > 0) ? 'height="' . $this->height . '"' : '';
		$img = '<img ' . $width . ' ' . $height . ' alt="' . $this->getTitle() . '" class="image ' . $class . '" src="' . $imgurl . '" ' . $style . " />";
    if (!empty($this->link)) {
		  $img = '<a href="#">' . $img . '</a>';
		} 
		return $img;

	}


  /**
   * Generate a thumbnail of an image.
   *
   * @param Environment $env
   *   The Environment.
   * @param array $vars
   *   An array of the variables for the thumbnail to generate.
   * @return string
   *   The url of the generated thumbnail.
   */
  public function generateThumbnail($env, $vars) {

    $maxw = isset($vars['w_max']) ? $vars['w_max'] : 0;
    $maxh = isset($vars['h_max']) ? $vars['h_max'] : 0;
    $image_action = isset($vars['image_action']) ? $vars['image_action'] : 0;
    $compression = isset($vars['compression']) ? $vars['compression'] : 0;
    $fallback = isset($vars['fallback']) ? $vars['fallback'] : 0;


    if ($maxw == 'auto') {
      $maxw = 0;
    }
    if ($maxh == 'auto') {
      $maxh = 0;
    }

    if (!(intval($maxw) > 0)) {
      $maxw = 999999;
    }
    if (!(intval($maxh) > 0)) {
      $maxh = 999999;
    }

    // Get the File path for the image
    $thumbRoot = $env->dir['thumbs'];
    $thumbfile = 'thumb-' . str_replace(' ', '-', str_replace('/', '-', $this->node->getName() . '-' . $this->width . 'x' . $this->height . '-' . $this->getName()));

    $sImagePath = $this->getRealPath();
    $thumbImagePath = $thumbRoot . '/' . $thumbfile;


    // TODO: a better cache system (refresh cache when image changes, also with same filename)
    // If thumbnail exists, use it.
    if (is_file($thumbImagePath)) {
			return $thumbfile;
    }
    
    // If the image file is broken, use the default broken image.
    if (!is_file($sImagePath)) {
      // Check if if set a fallback image.
      if (isset($fallback) && is_file($fallback)){
        // Set custom fallback image.
        $sImagePath = $fallback;
        $fallbackArr = explode('.', $fallback);
        $fallbackExtension = strtolower(end($fallbackArr));

        // New thumbfile.
        $thumbfile .= 'fallback.' . $fallbackExtension;
        $thumbImagePath = $thumbRoot . '/' . $thumbfile;
      } else {
        // Set Quanta default fallback image.
        $sImagePath = $this->env->getModulePath('image') . '/assets/broken-img.jpg';
      }
    }

    // If you want exact dimensions, you
    // will pass 'width' and 'height'

    $iThumbnailWidth = (int) $maxw;
    $iThumbnailHeight = (int) $maxh;


    // If you want proportional thumbnails,
    // you will pass 'maxw' and 'maxh'

    $iMaxWidth = (int) $maxw;
    $iMaxHeight = (int) $maxh;

    // Based on the above we can tell which
    // type of resizing our script must do



    // The 'scale' type will make the image
    // smaller but keep the same dimensions

    // The 'crop' type will make the thumbnail
    // exactly the width and height you choose

    // To start off, we will create a copy
    // of our original image in $img

    $img = NULL;

    // At this point, we need to know the
    // format of the image

    // Based on that, we can create a new
    // image using these functions:
    // - imagecreatefromjpeg
    // - imagecreatefrompng
    // - imagecreatefromgif

    $pathArr = explode('.', $sImagePath);
    $sExtension = strtolower(end($pathArr));

    if ($sExtension == 'jpg' || $sExtension == 'jpeg') {
      $img = imagecreatefromjpeg($sImagePath)
      or print("Cannot create new JPEG image: " . $sImagePath);
    } else if ($sExtension == 'png') {
      $img = imagecreatefrompng($sImagePath)
      or print("Cannot create new PNG image: " . $sImagePath);
    } else if ($sExtension == 'gif') {
      $img = imagecreatefromgif($sImagePath)
      or print("Cannot create new GIF image: " . $sImagePath);
    }

    // header("Content-type: image/" . $sExtension);


    // If the image has been created, we may proceed
    // to the next step

    if ($img) {

      // We now need to decide how to resize the image

      // If we chose to scale down the image, we will
      // need to get the original image propertions

      $iOrigWidth = imagesx($img);
      $iOrigHeight = imagesy($img);

      if ($image_action == 'scale') {

        // Get scale ratio

        $fScale = min($iMaxWidth/$iOrigWidth,
          $iMaxHeight/$iOrigHeight);

        // To explain how this works, say the original
        // dimensions were 200x100 and our max width
        // and height for a thumbnail is 50 pixels.
        // We would do $iMaxWidth/$iOrigWidth
        // (50/200) = 0.25
        // And $iMaxHeight/$iOrigHeight
        // (50/100) = 0.5

        // We then use the min() function
        // to find the lowest value.

        // In this case, 0.25 is the lowest so that
        // is our scale. The thumbnail must be
        // 1/4th (0.25) of the original image

        if ($fScale < 1) {

          // We must only run the code below
          // if the scale is lower than 1
          // If it isn't, this means that
          // the thumbnail we want is actually
          // bigger than the original image

          // Calculate the new height and width
          // based on the scale

          $iNewWidth = floor($fScale*$iOrigWidth);
          $iNewHeight = floor($fScale*$iOrigHeight);
          // Create a new temporary image using the
          // imagecreatetruecolor function

          $tmpimg = imagecreatetruecolor($iNewWidth,
            $iNewHeight);

          if ($sExtension == 'png') {
            //Preserve transparency when scaling PNG
            imagealphablending($tmpimg, false);
            imagesavealpha($tmpimg,true);
            $transparent = imagecolorallocatealpha($tmpimg, 255, 255, 255, 127);
            imagefilledrectangle($tmpimg, 0, 0, $iNewWidth, $iNewWidth, $transparent);
          }

          // The function below copies the original
          // image and re-samples it into the new one
          // using the new width and height

          imagecopyresampled($tmpimg, $img, 0, 0, 0, 0,
            $iNewWidth, $iNewHeight, $iOrigWidth, $iOrigHeight);

          // Finally, we simply destroy the $img file
          // which contained our original image
          // so we can replace with the new thumbnail

          imagedestroy($img);
          $img = $tmpimg;

        } else if ($sExtension == 'png') {
          //Preserve transparency for non scaled PNG
          imagealphablending($img, true);
          imagesavealpha($img,true);
        }

      } else if ($image_action == "crop") {

        // Get scale ratio

        $fScale = max($iThumbnailWidth/$iOrigWidth,
          $iThumbnailHeight/$iOrigHeight);

        // This works similarly to other one but
        // rather than the lowest value, we need
        // the highest. For example, if the
        // dimensions were 200x100 and our thumbnail
        // had to be 50x50, we would calculate:
        // $iThumbnailWidth/$iOrigWidth
        // (50/200) = 0.25
        // And $iThumbnailHeight/$iOrigHeight
        // (50/100) = 0.5

        // We then use the max() function
        // to find the highest value.

        // In this case, 0.5 is the highest so that
        // is our scale. This is the first step of
        // the image manipulation. Once we scale
        // the image down to 0.5, it will have the
        // dimensions of 100x50. At this point,
        // we will need to crop the image, leaving
        // the height identical but halving
        // the width to 50

        if ($fScale < 1) {

          // Calculate the new height and width
          // based on the scale

          $iNewWidth = floor($fScale*$iOrigWidth);
          $iNewHeight = floor($fScale*$iOrigHeight);
          // Create a new temporary image using the
          // imagecreatetruecolor function

          $tmpimg = imagecreatetruecolor($iNewWidth,
            $iNewHeight);
          $tmp2img = imagecreatetruecolor($iThumbnailWidth,
            $iThumbnailHeight);

          if ($sExtension == 'png') {
            //Preserve transparency when scaling & cropping PNG
            //$tmpimg
            imagealphablending($tmpimg, false);
            imagesavealpha($tmpimg,true);
            $transparent = imagecolorallocatealpha($tmpimg, 255, 255, 255, 127);
            imagefilledrectangle($tmpimg, 0, 0, $iNewWidth, $iNewWidth, $transparent);
            //$tmp2img
            imagealphablending($tmp2img, false);
            imagesavealpha($tmp2img,true);
            $transparent = imagecolorallocatealpha($tmp2img, 255, 255, 255, 127);
            imagefilledrectangle($tmp2img, 0, 0, $iNewWidth, $iNewWidth, $transparent);            
          }

          // The function below copies the original
          // image and re-samples it into the new one
          // using the new width and height

          imagecopyresampled($tmpimg, $img, 0, 0, 0, 0,
            $iNewWidth, $iNewHeight, $iOrigWidth, $iOrigHeight);

          // Our $tmpimg will now have the scaled down
          // image. The next step is cropping the picture to
          // make sure it's exactly the size of the thumbnail

          // The following logic choose how the image
          // will be cropped. Using the previous example, it
          // needs to take a 50x50 block from the original
          // image and copy it over to the new thumbnail

          // Since we want to copy the exact center of the
          // scaled down image, we need to find out the x
          // axis and y axis. To do so, say the scaled down
          // image now has a width of 100px but we want it
          // to be only 50px

          // Somehow, we need to select between the 25th and
          // 75th pixel to copy the middle.

          // To find this value we do:
          // ($iNewWidth/2)-($iThumbnailWidth/2)

          // ( 100px / 2 ) - (50px / 2)
          // ( 50px ) - ( 25px )
          // = 25px

          $xAxis = 0;
          $yAxis = 0;

          if ($iNewWidth == $iThumbnailWidth) {
            $yAxis = ($iNewHeight/2)-
              ($iThumbnailHeight/2);
            $xAxis = 0;
          }
          else if ($iNewHeight == $iThumbnailHeight)  {
            $yAxis = 0;
            $xAxis = ($iNewWidth/2)-
              ($iThumbnailWidth/2);
          }

          // We now have to resample the new image using the
          // new dimensions are axis values.

          imagecopyresampled($tmp2img, $tmpimg, 0, 0,
            $xAxis, $yAxis,
            $iThumbnailWidth,
            $iThumbnailHeight,
            $iThumbnailWidth,
            $iThumbnailHeight);

          imagedestroy($img);
          imagedestroy($tmpimg);
          $img = $tmp2img;

        }
        else if ($sExtension == 'png') {
          //Preserve transparency for non cropped PNG
          imagealphablending($img, true);
          imagesavealpha($img,true);
        }
      }

      // Display the image using the header function to specify
      // the type of output our page is giving
      if ($sExtension == 'jpg' || $sExtension == 'jpeg') {
        imagejpeg($img, $thumbImagePath, $compression);
      } else if ($sExtension == 'png') {
        imagepng($img, $thumbImagePath, ($compression / 10));
      } else if ($sExtension == 'gif') {
        imagegif($img, $thumbImagePath);
      }

    }
    return $thumbfile;
  }
} 
