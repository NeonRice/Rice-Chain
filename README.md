# 🍡 Rice-Chain 🍡
What if you made a chain out of your rice?

## <i>📰 Instructions 📰</i>

1. Download the source code
2. Make sure you set THREADS variable ( in Miner.h ) to the amount of threads your CPU has
3. Compile the .cpp files with -lpthread in src and external folders
4. Run the compiled program

## <i>♨️ Main idea ♨️</i>
<font size="2">

What we're trying to achieve here is a simplistic replication of the block-chain technology.

We generate **users** and **transactions** that we put into the **transaction pool**. Then we pick **n** amount of transactions randomly from the transaction pool and use **n / THREADS** of them for **each thread** running in **parallel** to try and find a block with the hash that is **adequate** for our **difficulty target**. Then we **validate** and **execute** all of the **valid transactions** in our **mined block**. 

We **repeat** this process until **transaction pool** becomes **empty**.
</font>


## <i>🏁 Speed test 🏁</i>

Mining was done with **12 Threads i7 8086k CPU**

Test score of 100 **transactions** per **block**

| Difficulty | Average Time To Mine 1 Block |
| ---------- | ---------------------------- |
| 4          | 50 seconds                   |
| 3          | 4 seconds                    |
| 2          | 0.3 seconds                  |

## <i> ⛏️ Thread Mining ⛏️ </i>

Test done with **complexity** of 3, 100 **blocks** of 100 **transactions**

| Thread Nr | Mining Tries | Successful Tries |
| --------- | ------------ | ---------------- |
| 0         | 36297        | 12               |
| 1         | 36291        | 7                |
| 2         | 36559        | 11               |
| 3         | 36444        | 6                |
| 4         | 36428        | 8                |
| 5         | 36336        | 9                |
| 6         | 36295        | 6                |
| 7         | 36444        | 4                |
| 8         | 36284        | 11               |
| 9         | 36412        | 8                |
| 10        | 36428        | 12               |
| 11        | 36269        | 6                |

## <i>Final words ✔️</i>
<font size="2">
Block-chain technology good. Smile

All of the 💕 to my [🍣](https://github.com/Definitelynotaspruce) girl  for her support and love
</font>

 ## <i>Releases</i>
 
[**[v0.1]**](https://github.com/NeonRice/Rice-Chain/releases/tag/0.1) 