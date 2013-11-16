if (typeof login !="undefined" && typeof password !="undefined") {
    $login = $("input[name=username]");
    $password = $("input[name=password]");
    $submit = $("button[type=submit]");
    if ($login.size()
            && $password.size()
            && $submit.size()
            && !$("ul.errorlist").size()) {
        $login.val(login);
        $password.val(password);
        $submit.click();
    }
}
