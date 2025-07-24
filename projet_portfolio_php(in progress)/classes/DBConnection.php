<?php

class DBConnection
{
    private $host = HOST;
    private $dbusername = DB_USERNAME;
    private $password = DB_PASSWORD;
    private $database = DB_NAME;
    public $conn;
    public function __construct()
    {
        if (!isset($this->conn)) {

            $this->conn = new PDO("mysql:host=$this->host;dbname=$this->database", $this->dbusername, $this->password);
            if (!$this->conn) {
                echo 'Cannot connect to database server';
                exit;
            }
        }
    }

    public function __destruct(){
        $this->conn = null;
    }
}