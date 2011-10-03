(function ($) {

Drupal.behaviors.stripeConfig = {
  attach: function (context) {
    // For now we'll just do the test form.
    $("#stripe-admin-test").submit(function(event) {
        console.log('test');
        // disable the submit button to prevent repeated clicks
        $('.submit-button').attr("disabled", "disabled");

        var amount = $('.amount').val();
        Stripe.createToken({
            number: $('.card-number').val(),
            cvc: $('.card-cvc').val(),
            exp_month: $('.card-expiry-month').val(),
            exp_year: $('.card-expiry-year').val()
        }, amount, stripeResponseHandler);

        // prevent the form from submitting with the default action
        return false;
    });
  }
};



  stripeResponseHandler = function(status, response) {
    console.log(response);
    if (response.error) {
      //show the errors on the form
      $("#stripe-admin-test").after($('<div class="payment-errors"></div>').html(response.error.message));
    }
    else {
      var $form = $("#stripe-admin-test");
      // token contains id, last4, and card type
      var token = response['id'];
      // insert the token into the form so it gets submitted to the server
      $form.append("<input type='hidden' name='stripeToken' value='" + token + "'/>");
      // and submit
      //$form.get(0).submit();
      alert('done');
    }
  }

})(jQuery);