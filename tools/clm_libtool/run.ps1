$action = $args[0]
$name = $args[1]

function PrepLibTool {
    param ($name)

    poetry run python "src/clm_libtool.py" --root_path "../../." --action "prepare" --name $name
}

function CleanLibTool {
    param ($name)

    poetry run python "src/clm_libtool.py" --root_path "../../." --action "clean" --name $name
}

function UpdateLibTool {
    param ($name)

    poetry run python "src/clm_libtool.py" --root_path "../../." --action "update" --name $name
}

Switch ($action) {
    "prepare" { PrepLibTool($name); Break }
    "clean" { CleanLibTool($name); Break }
    "update" { UpdateLibTool($name); Break }
}
