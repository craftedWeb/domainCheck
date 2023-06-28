$(document).ready(function() {
    $('.domainCheck form').submit(function(e) {
        e.preventDefault(); // Voorkom standaard formulierverzending

        let domainName = $('#domain-input').val(); // Haal de waarde van het domeinveld op

        $.ajax({
            url: 'domaincheck.php',
            type: 'POST',
            data: { domain: domainName },
            success: function(response) {
                $('#availability-message').html(response); // Plaats de response in het availability-message element
            },
            error: function(xhr, status, error) {
                console.log(xhr.responseText); // Toon eventuele fouten in de console
            }
        });
    });
});
