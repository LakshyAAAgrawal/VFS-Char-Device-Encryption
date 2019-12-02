#!/bin/bash
sudo rmmod encdev
sudo insmod encdev.ko
sudo rmmod decdev
sudo insmod decdev.ko
