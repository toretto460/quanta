<?php
/**
 * Implements qtag SEARCH.
 *
 * Creates a search form.
 *
 * @param Environment $env
 *   The Environment.
 *
 * @param string $target
 *   The qtag's target.
 *
 * @param array $attributes
 *   The qtag's attributes.
 *
 * @return string
 *   The rendered qtag.
 */
function qtag_SEARCH($env, $target, &$attributes) {
  $search_form = '
		[INPUT|name=search|title=|type=string:search-form]
		[INPUT|name=submit|type=submit|value=Search!:search-form]
		[FORM|method=get|action=' . $target . ':search-form]';
  return $search_form;
}

/**
 * Implements qtag SEARCHRESULTS.
 *
 * Renders the results of a search.
 *
 * @param Environment $env
 *   The Environment.
 *
 * @param string $target
 *   The qtag's target.
 *
 * @param array $attributes
 *   The qtag's attributes.
 *
 * @return string
 *   The rendered qtag.
 */
function qtag_SEARCHRESULTS($env, $target, &$attributes) {
  $output = '';
  $results = array();
  $results_arr = search($env);
  $output .= '<div>You are searching for: <b>' . $_REQUEST['search'] . ' </div>
    <div id="data" class="search-results">';
  if (count($results_arr) == 0) {
    $output .= '<div class="search-no-results">No results found</div>';
  }
  // TODO: move into a Search class.
  // TODO: implement ElasticSearch.
  foreach ($results_arr as $k => $result_string) {
    $r = explode(':' , $result_string);
    $result = preg_replace('/(' . $_REQUEST['search'] . ')/is', '<b class="search-result-match">$1</b>', strip_tags(nl2br(str_replace($r[0], '', $r[1]))));
    $filepath_exp = explode('/', $r[0]);
    $result_pagename = $filepath_exp[count($filepath_exp) - 2];
    if (substr($result_pagename, 0, 1) != '_') {
      $results[$result_pagename][] = '<div class="search-result-string">... ' . preg_replace('/\[(.*?)\]/', '', trim($result)) . ' ...</div>';
    }
  }
  $counter = array();
  $total_files = 0;
  $total_matches = 0;

  foreach ($results as $pname => $v) {
    $c = count($results[$pname]);
    $total_files++;
    $total_matches += $c;
    $counter[$pname] = $c;
  }
  arsort($counter);
  foreach ($counter as $pname => $result_count) {
    $output .= '<div class="search-result">' . $results[$pname][0] . '<div class="search-result-file"><b>' . $result_count. '</b> <span translate="yes">results found in:</span> [LINK:' . $pname . ']</div></div>';
  }

  $output .= '<div class="search-results-total">Total: <b>' . $total_matches . '</b> matches found in <b>' . $total_files . '</b> files.</div>';
  $output .= '</div>';

  return $output;
}

