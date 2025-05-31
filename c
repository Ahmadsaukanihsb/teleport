local Players = game:GetService("Players")
local ReplicatedStorage = game:GetService("ReplicatedStorage")
local TextChatService = game:GetService("TextChatService")

-- Configuration
local GIFT_COOLDOWN = 5
local lastGiftCheck = 0

local function acceptGiftsAutomatically()
    if os.time() - lastGiftCheck < GIFT_COOLDOWN then return false end
    lastGiftCheck = os.time()

    -- Method 1: Direct RemoteEvent triggering
    local remoteNames = {
        "AcceptGiftRemote", "GiftAcceptRemote", "GiftSystemRemote",
        "GiftEvent", "GiftNetwork", "GiftHandler", "ReceiveGift"
    }

    for _, name in pairs(remoteNames) do
        local remote = ReplicatedStorage:FindFirstChild(name)
        if remote and remote:IsA("RemoteEvent") then
            pcall(function()
                -- Try various possible parameter combinations
                remote:FireServer()
                remote:FireServer("accept")
                remote:FireServer(true)
                remote:FireServer("accept_all")
                remote:FireServer(Players.LocalPlayer)
                print("🎁 Successfully accepted gifts via RemoteEvent:", name)
                return true
            end)
        end
    end

    -- Method 2: BindableEvents in PlayerGui
    local gui = Players.LocalPlayer:FindFirstChildOfClass("PlayerGui")
    if gui then
        for _, bindable in ipairs(gui:GetDescendants()) do
            if bindable:IsA("BindableEvent") and string.find(string.lower(bindable.Name), "gift") then
                pcall(function()
                    bindable:Fire()
                    bindable:Fire("accept")
                    print("🎁 Success via BindableEvent:", bindable.Name)
                    return true
                end)
            end
        end
    end

    -- Method 3: Gift data in player attributes
    if Players.LocalPlayer:GetAttribute("PendingGifts") then
        pcall(function()
            ReplicatedStorage.GiftSystem:FireServer("accept_all")
            print("🎁 Accepted via pending gifts attribute")
            return true
        end)
    end

    -- Method 4: Find and accept specific gift instances
    local giftsFolder = Players.LocalPlayer:FindFirstChild("Gifts")
    if giftsFolder then
        for _, gift in ipairs(giftsFolder:GetChildren()) do
            pcall(function()
                ReplicatedStorage.GiftSystem:FireServer("accept", gift.Name)
                print("🎁 Accepted gift:", gift.Name)
            end)
        end
        return true
    end

    print("⚠️ No automatic gift acceptance method worked")
    return false
end

-- Main loop
while task.wait(5) do
    pcall(acceptGiftsAutomatically)
    
    -- Optional chat message to keep instance active
    if math.random(1, 10) == 1 then
        pcall(function()
            TextChatService.TextChannels.RBXGeneral:SendAsync("Auto-gift farming active")
        end)
    end
end
