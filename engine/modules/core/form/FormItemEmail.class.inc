<?php
/**
 * Class FormItemEmail
 * This class represents a Form Item of type Email.
 */
class FormItemEmail extends FormItemString {
  public $type = 'email';

  public function loadAttributes() {
    parent::loadAttributes(); // TODO: Change the autogenerated stub
  }

  /**
   * @return string
   */
  public function getHTMLType() {
    return 'email';
  }

  /**
   *
   */
  public function validate() {
    $value = $this->getValue();
    if (!valid_email(array_pop($value))) {
       $this->getForm()->validationError($this, 'Insert a valid email');
    };
  }
}