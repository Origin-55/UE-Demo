# UE-Demo
## 项目概述
这是一个 Unreal Engine 5 的 C++ 学习项目
## 核心架构
两对象对象池（角色重生系统）

项目使用一个简单的两对象对象池来实现角色重生：
• AMyGameModeBase：管理两个角色实例

◦ ActiveCharacter：当前被玩家控制的角色

◦ PooledCharacter：备用角色（隐藏、无碰撞、无Tick）

◦ SwapCharacter(PC)：交换角色（重生核心逻辑）

◦ bIsSwaped：防止重复交换的标志

• AThirdCharacter：玩家角色类，继承 ACharacter 并实现 IBPI_Death 接口

◦ Death_Implementation()：处理死亡（开启物理模拟、禁用输入）

◦ Respawn()：3秒后调用 SwapCharacter 进行重生

◦ Activate() / Deactivate()：控制角色的激活/停用状态

◦ ResetCharacter()：重置角色状态（关闭物理、恢复行走模式）

• AMyPlayerState：存储复活点变换信息

◦ SpawnPointTransform：当前复活点的位置、旋转和缩放

• AMySpawnPoint：关卡中的检查点，角色触碰后更新复活点

## 角色生命周期流程
生成 → BeginPlay（创建两个角色，Active激活，Pooled停用）→
  
Active角色死亡 → Ragdoll → 3秒计时器 →
 
Respawn() → SwapCharacter() →
 
Pooled角色：ResetCharacter → SetActorTransform(复活点) → Activate → 被Possess →

原Active角色：Deactivate → 变为新的Pooled →
 
指针交换：Active ↔ Pooled

## 重要注意事项

1. DefaultPawnClass = nullptr：游戏模式不自动生成Pawn，由 BeginPlay() 手动创建
2. SwapCharacter 中必须先调用 ResetCharacter() 再调用 SetActorTransform()：因为退出 Ragdoll 时 UE 会根据 Mesh 骨骼位置重新定位胶囊体，如果顺序反了会覆盖传送位置
3. Deactivate() 中必须停止物理模拟：防止池中角色在隐藏期间继续 Ragdoll 坠落
4. 复活点通过 PlayerState 传递：初始值为 BeginPlay 时的玩家出生点，可通过 MySpawnPoint 更新

