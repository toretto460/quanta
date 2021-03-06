<?php

/**
 * Transform qtags in html.
 * Will look for all qtag_TAG functions, in all modules, and apply them to the code
 * to convert tags.
 *
 * TODO: convert to OO approach.
 *
 * @param Environment $env
 *   The Environment.
 *
 * @param string $html
 *   The html to analyze.
 *
 * @param array $options
 *   Options for qtags.
 *
 * @param string $regex_options
 *   Options for the regex closure.
 *
 * @return array
 *   All the qtags in the html.
 *
 */
function checkCodeTags(&$env, $html, $options = array(), $regex_options = 'U') {
  $replacing = array();
  // Find all qtags using regular expressions (both { and [ bracket types are valid for now).
  // TODO: allow more qtag open / close characters, for advanced nesting.
  $regexs = array();

  $qtag_delimiters = isset($options['qtag_delimiters']) ? $options['qtag_delimiters'] : array('[]', '{}');
  foreach ($qtag_delimiters as $qtag_delimiter) {
    $qtag_del_open = substr($qtag_delimiter, 0, 1);
    $qtag_del_close = substr($qtag_delimiter, 1, 1);

    $regexs['/\\' . $qtag_del_open . '(.*)\\' . $qtag_del_close . '/' . $regex_options] = array($qtag_del_open, $qtag_del_close, '|');
  }

  // Run the regular expression: find all the qtags in the page.
  foreach ($regexs as $regex => $delimiters) {
    preg_match_all($regex, $html, $matches);
    foreach ($matches[1] as $tag_value) {
      $tag_full = $delimiters[0] . $tag_value . $delimiters[1];
      $replaces = checkCodeTags($env, $tag_value, $options);
      foreach ($replaces as $tag => $val) {
        if (is_array($val)) {
          $val = implode(GLOBAL_SEPARATOR, $val);
        }
        $tag_value = str_replace($tag, $val, $tag_value);
      }
      $tag = explode(':', $tag_value);

      // If there is more than one : we have to just consider the first chunk
      // and unify the rest.

      $tag_destination = isset($tag[1]) ? $tag[1] : NULL;
      for ($i = 2; $i < count($tag); $i++) {
        $tag_destination .= ':' . $tag[$i];
      }

      // Load the attributes of the qtag.
      $attributes = explode($delimiters[2], $tag[0]);
      $tag_name = $attributes[0];
      $attr_arr = array();
      unset($attributes[0]);
      // Assign attributes as specified in the tag.
      foreach ($attributes as $attr_item) {
        $split = explode('=', $attr_item);

        // If there is more than one = we have to just consider the first chunk
        // and unify the rest.

        $attribute_name = $split[0];
        $attribute_value = isset($split[1]) ? $split[1] : NULL;
        for ($i = 2; $i < count($split); $i++) {
          $attribute_value .= '=' . $split[$i];
        }

        if (isset($attribute_value)) {
          $attr_arr[$attribute_name] = $attribute_value;
        }
        else {
          $attr_arr[$attribute_name] = TRUE;
        }
      }

      // When the highlight attribute is used, don't render the tab but display it in an highlighted format.
      // TODO: not elegant to have this here?
      if (!empty($attr_arr['highlight'])) {
        $replacing[$tag_full] = _qtag_highlight($tag_name, $attributes, $tag_destination, $delimiters);
      }
      // When the showtag attribute is used, don't render the tab but just display it.
      elseif (!empty($attr_arr['showtag'])) {
        $replacing[$tag_full] =  string_normalize(str_replace('|showtag', '', $tag_full));
      }
      // When the showtag attribute is used, don't render the tab but just display it.
      elseif (!empty($attr_arr['runlast']) && empty($options['runlast'])) {

        continue;
      }
      else {

        // Check if the requested qtag is defined by any function.
        if ($tag_name == strtoupper($tag_name) && function_exists('qtag_' . $tag_name)) {
          $func = 'qtag_' . $tag_name;
          if (function_exists($func)) {
            $qtag[$tag_full] = '';
            $attr_arr['tag_full'] = $tag_full;
            $qtag_access = TRUE;

            $vars = array(
              'attributes' => &$attr_arr,
              'target' => $tag_destination,
              'qtag' => &$qtag[$tag_full],
              'access' => &$qtag_access,
            );

            $env->hook('qtag_preload', $vars);

            // Check that access is OK for the qtag.
            if (!$qtag_access) {
              $replacing[$tag_full] = '';
            }
            else {

              // Run the qtag.
              $tag_html = $func($env, !empty($tag_destination) ? $tag_destination : NULL, $vars['attributes']);

              // Prepare the qtag replacement.
              $qtag[$tag_full] = $tag_html;

              // Let other modules hook into the rendered qtag.
              $env->hook('qtag', $vars);

              $tag_content = $qtag[$tag_full];

              // Add eventual suffix and prefix.
              if (!empty($attr_arr['suffix']) && !empty($tag_content)) {
                $tag_content .= $attr_arr['suffix'];
              }
              if (!empty($attr_arr['prefix']) && !empty($tag_content)) {
                $tag_content = $attr_arr['prefix'] . $tag_content;
              }

              // Prevent replacement where no_qtags attribute present. Used for input forms etc.
              if (isset($attr_arr['no_qtags'])) {
                $tag_content = string_normalize($tag_content);
              }

              // Add the rendered qtag to the replacements.
              $replacing[$tag_full] = $tag_content;
            }
          }
        }
      }
    }
  }

  return $replacing;
}

/**
 * Replace all the qTags in the page into their HTML equivalent.
 *
 * @param Environment $env
 *   The Environment.
 *
 * @param string $html
 *   The current HTML
 *
 * @param array $options
 *   Other options.
 *
 * @return mixed
 *   The HTML with all qTags transformed.
 */
function transformCodeTags(&$env, $html, $options = array()) {

  // After rendering all tags in a page, the result could be containing other tags.

  // For this reason, keep looping until all tags are rendered.
  while (TRUE) {

    $replaces = (checkCodeTags($env, $html, $options));

    if (empty($replaces)) {
      break;
    }
    foreach ($replaces as $k => $replace) {
      if (is_array($replace)) {
        $replace = implode(GLOBAL_SEPARATOR, $replace);
      }
      $html = str_replace($k, $replace, $html);
    }

  }
  return $html;
}


/**
 * Remove all qtags elements from the string (all [elements] within brackets).
 *
 * @param string $string
 *   The string to be stripped.
 *
 * @return string
 *   The stripped string.
 */
function stripQTags($string) {
  return preg_replace('/\[.*?\]/', '', $string);
}
  