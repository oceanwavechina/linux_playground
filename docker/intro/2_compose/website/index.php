<<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <title>My Shop</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" type="text/css" media="screen" href="main.css">
    <script src="main.js"></script>
</head>
<body>
    <H1>Welcome to my shop</H1>
    <ul>
        <?php
            $json = file_get_contents("http://product-service");
            $obj = json_decode($json);

            $products = $obj->products;
            foreach($products as $product) {
                echo "<li>$product</li>";
            }
        ?>
    </ul>
</body>
</html>