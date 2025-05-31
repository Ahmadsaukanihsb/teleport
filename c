local Players = game:GetService("Players")
local ReplicatedStorage = game:GetService("ReplicatedStorage")

local GIFT_COOLDOWN = 5
local lastGiftCheck = 0

local function acceptItemGifts()
    local player = Players.LocalPlayer
    if not player then return false end

    if os.time() - lastGiftCheck < GIFT_COOLDOWN then return false end
    lastGiftCheck = os.time()

    local giftsFolder = player:FindFirstChild("Gifts") or player:FindFirstChild("ReceivedGifts")
    if not giftsFolder then
        --print("Tidak ada folder Gifts atau ReceivedGifts")
        return false
    end

    for _, gift in ipairs(giftsFolder:GetChildren()) do
        if gift:IsA("Folder") or gift:IsA("ValueBase") or gift:IsA("Instance") then
            local giftName = gift.Name
            local remotes = {
                ReplicatedStorage:FindFirstChild("GiftSystem"),
                ReplicatedStorage:FindFirstChild("GiftHandler"),
                ReplicatedStorage:FindFirstChild("AcceptGiftRemote"),
                ReplicatedStorage:FindFirstChild("GiftAcceptRemote"),
            }

            for _, remote in ipairs(remotes) do
                if remote and remote:IsA("RemoteEvent") then
                    local success, err = pcall(function()
                        remote:FireServer("accept", giftName)
                    end)
                    if success then
                        print("🎁 Gift diterima:", giftName, "via", remote.Name)
                        break
                    else
                        warn("Gagal terima gift", giftName, "Error:", err)
                    end
                end
            end
        end
    end

    return true
end

-- Loop terima gift otomatis tiap 5 detik
while task.wait(GIFT_COOLDOWN) do
    pcall(acceptItemGifts)
end
