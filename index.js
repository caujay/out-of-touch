const express = require("express");
const dgram = require("dgram");
const touchSensor = dgram.createSocket("udp4");
const bendlabs = dgram.createSocket("udp4");
const movement = dgram.createSocket("udp4");
const path = require("path");

const app = express();

app.use(
  express.urlencoded({
    extended: true,
  })
);

let gloveIP = null;
let roboticArmIP = null;

const resetValues = () => {
  gloveIP = null;
  roboticArmIP = null;
};

app.use(express.static(path.join(__dirname, "/public")));

app.get("/", function (req, res) {
  res.sendFile(__dirname + "/public/index.html");
});

app.post("/test", function (req, res) {
  gloveIP = req.body.glove;
  roboticArmIP = req.body.arm;
  console.log(`glove = ${gloveIP}, rob = ${roboticArmIP}`);
  if (gloveIP != "" && roboticArmIP != "") {
    res.sendFile(__dirname + "/public/sucessful.html");
  } else {
    resetValues();
    console.log("empty values");
    res.sendFile(__dirname + "/public/fail.html");
  }
});

app.post("/close", function (req, res) {
  resetValues();
  console.log("values set to null, server transmission stopped");
  res.sendFile(__dirname + "/public/close.html");
});

app.listen(3000, "192.168.1.11", function () {
  console.log("Server is running");
});

//touch sensor
// if (gloveIP != null && roboticArmIP != null) {
touchSensor.on("error", (err) => {
  console.log(`server error:\n${err.stack}`);
  touchSensor.close();
});

touchSensor.on("message", (msg, rinfo) => {
  console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
  touchSensor.send(msg, 3004, "192.168.1.44", (err) => {
    console.log(`msg is: ${msg.toString("utf-8")}`);
  });
});

touchSensor.on("listening", () => {
  const address = touchSensor.address();
  console.log(
    `touch sensor server listening ${address.address}:${address.port}`
  );
});

touchSensor.bind(3001, "192.168.1.11");

//bendlabs

bendlabs.on("error", (err) => {
  console.log(`server error:\n${err.stack}`);
  bendlabs.close();
});

bendlabs.on("message", (msg, rinfo) => {
  console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
  bendlabs.send(msg, 3005, "192.168.1.38", (err) => {
    console.log(`msg is: ${msg.toString("utf-8")}`);
  });
});

bendlabs.on("listening", () => {
  const address = bendlabs.address();
  console.log(`twist server listening ${address.address}:${address.port}`);
});

bendlabs.bind(3002, "192.168.1.11");

//movement

movement.on("error", (err) => {
  console.log(`server error:\n${err.stack}`);
  movement.close();
});

movement.on("message", (msg, rinfo) => {
  console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
  movement.send(msg, 3006, "192.168.1.37", (err) => {
    console.log(`msg is: ${msg.toString("utf-8")}`);
  });
});

movement.on("listening", () => {
  const address = movement.address();
  console.log(`movement server listening ${address.address}:${address.port}`);
});

movement.bind(3003, "192.168.1.11");
// }
