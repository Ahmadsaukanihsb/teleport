local Players = game:GetService("Players")
local ReplicatedStorage = game:GetService("ReplicatedStorage")

local GIFT_COOLDOWN = 5
local lastGiftCheck = 0

local function acceptItemGifts()
    local player = Players.LocalPlayer
    if not player then 
        print("LocalPlayer belum siap")
        return false 
    end

    if os.time() - lastGiftCheck < GIFT_COOLDOWN then
        print("Cooldown gift belum selesai")
        return false 
    end
    lastGiftCheck = os.time()

    local giftsFolder = player:FindFirstChild("Gifts") or player:FindFirstChild("ReceivedGifts")
    if not giftsFolder then
        print("Folder Gifts atau ReceivedGifts tidak ditemukan pada player")
        return false
    end

    print("Folder gifts ditemukan, jumlah hadiah:", #giftsFolder:GetChildren())

    for _, gift in ipairs(giftsFolder:GetChildren()) do
        if gift:IsA("Folder") or gift:IsA("ValueBase") or gift:IsA("Instance") then
            local giftName = gift.Name
            print("Mendapat gift:", giftName)

            local remotes = {
                ReplicatedStorage:FindFirstChild("GiftSystem"),
                ReplicatedStorage:FindFirstChild("GiftHandler"),
                ReplicatedStorage:FindFirstChild("AcceptGiftRemote"),
                ReplicatedStorage:FindFirstChild("GiftAcceptRemote"),
            }

            local accepted = false
            for _, remote in ipairs(remotes) do
                if remote and remote:IsA("RemoteEvent") then
                    print("Mencoba terima gift via remote:", remote.Name)
                    local success, err = pcall(function()
                        remote:FireServer("accept", giftName)
                    end)
                    if success then
                        print("🎁 Gift diterima:", giftName, "via", remote.Name)
                        accepted = true
                        break
                    else
                        warn("Gagal terima gift", giftName, "Error:", err)
                    end
                end
            end

            if not accepted then
                print("Gagal menerima gift:", giftName, "- remote tidak cocok atau error")
            end
        end
    end

    return true
end

while task.wait(GIFT_COOLDOWN) do
    pcall(acceptItemGifts)
end
