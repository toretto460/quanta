<?php
/**
 * Created by PhpStorm.
 * User: aldotripiciano
 * Date: 10/05/15
 * Time: 09:57
 */

function box_init($vars) {
  $vars['env']->addInclude('engine/code/php/core/box/js/box.js');
  $vars['env']->addInclude('engine/code/php/core/box/css/box.css');
}

/**
 * Render a box.
 */
function box_wikitag($vars) {
// TODO: move all this logic in a Box class.
  if (!empty($vars['attributes']['box'])) {

    $boxsize = explode('x', $vars['attributes']['box']);

    if ($boxsize[0] == 'auto') {
      $w = '';
    }
    elseif ($boxsize[0] > 0) {
      $w = 'w' . $boxsize[0];
    }
    else {
      // TODO: wrong box size.
    }

    if ($boxsize[1] == 'auto') {
      $h = '';
    }
    elseif ($boxsize[1] > 0) {
      $h = 'h' . $boxsize[1];
    }
    else {
      // TODO: wrong box size.
    }

    $name = $vars['attributes']['name'];
    $clear = !empty($vars['attributes']['clear']) ? 'clear' : '';

    foreach ($vars['wikitag'] as $tag => $content) {
      // TODO: move in tpl file.

      if (!empty($vars['attributes']['title'])) {
        $content = '<h2 class="box-title">' . $vars['attributes']['title'] . '</h2>' . $content;
      }
      $content = '<div class="inner">' . $content . '</div>';

      $vars['wikitag'][$tag] = '<div class="box ' . $w . ' ' . $h . ' ' . $name .' ' . $clear . '">' . $content . '</div>';
    }
  }
}