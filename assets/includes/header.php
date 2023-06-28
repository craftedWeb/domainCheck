<?php
echo "
<!doctype html>
    <html lang='nl'>
        <head>
            <meta charset='UTF-8'>
            <meta name='viewport' content='width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0'>
            <meta http-equiv='X-UA-Compatible' content='ie=edge'>";

            if (isset($pagetitle)) {
                echo '<title>' . $pagetitle . '</title>';
            } else {
                echo '<title>Ontwikkelaar vergat een titel</title>';
            }

            if (isset($pagekeywords)) {
                if (is_array($pagekeywords)) {
                    $keywords = implode(', ', $pagekeywords);
                    echo "<meta name='keywords' content='$keywords'>";
                } else {
                    echo "<meta name='keywords' content='$pagekeywords'>";
                }
            } else {
                echo "<meta name='keywords' content='Wake up dev! You have no keywords!'>";
            }

            if (isset($pagedescription)) {
                echo "<meta name='description' content='$pagedescription'>";
            } else {
                echo "<meta name='description' content='Geen beschrijving? Da\"s saai'>";
            }

            echo "
            <link rel='icon' type='image/png' href='/assets/images/favicons/favicon.ico'>
            <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.3/css/all.min.css'>
            <link rel='stylesheet' href='/assets/styles/styles.min.css'>
            <script src='https://kit.fontawesome.com/317092bc1a.js' crossorigin='anonymous'></script>
            <script src='/assets/jquery/jquery-3.6.3.min.js'></script>
            <script async src='https://www.googletagmanager.com/gtag/js?id=G-8R35Y6VF2B'></script>
            <link rel='preconnect' href='https://fonts.googleapis.com'>
            <link rel='preconnect' href='https://fonts.gstatic.com' crossorigin>
            <link href='https://fonts.googleapis.com/css2?family=Rubik:ital,wght@0,300;0,400;0,500;0,600;0,700;0,800;0,900;1,300;1,400;1,500;1,600;1,700;1,800;1,900&display=swap' rel='stylesheet'>
        </head>
            
        <body class='" . str_replace(' ', '-', $pagetitle) . "'>
            <header>
                <div class='headerContainer'>
                    <div class='logo'>
                        <a style='text-decoration: none; font-size: 1.2em' href='/'>
                            
                        </a>
                    </div>
                    <div class='hamburger'>
                        <i class='fas fa-bars'></i>
                    </div>
                    <nav>
                        <ul>
                            <li>
                                <a href='/#'>Home</a>
                            </li>
                            <li>
                                <a href='/pages/projecten/'>Projecten</a>
                            </li>
                            <li>
                                <a href='/pages/diensten'>Diensten</a>
                            </li>
                            <li>
                                <a href='/pages/over-ons'>Over&nbsp;ons</a>
                            </li>
                            <li>
                                <a href='/pages/contact'>Contact</a>
                            </li>
                            <div class='socialIcons'>
                                <li>
                                    <a href='https://www.linkedin.com/company/YourLinkedin/'><i style='color: #0A66C2' class='fab fa-linkedin'></i></a>
                                </li>
                                <li>
                                    <a href='https://www.facebook.com/yourFacebook'><i style='color: #1877F2;' class='fab fa-facebook'></i></a>
                                </li>
                                <li>
                                    <a href='https://github.com/craftedWeb/'><i class='fab fa-github'></i></a>
                                </li>
                            </div>
                        </ul>
                    </nav>
                </div>
            </header>            
            ";
